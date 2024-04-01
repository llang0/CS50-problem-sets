import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime, timezone

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    user_id = session["user_id"]
    # cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
    cash = db.execute("SELECT cash FROM users WHERE id = ?", user_id)[0]["cash"]
    total = cash

    if db.execute("SELECT * FROM transactions WHERE user_id = ?", user_id):
        stocks = db.execute("SELECT symbol, SUM(shares) AS shares FROM transactions WHERE user_id = ? GROUP BY symbol HAVING SUM(shares) > 0", user_id)
    else:
        stocks = []
    data = []

    for stock in stocks:
        # shares = db.execute("SELECT SUM(shares) FROM transactions WHERE symbol = ? AND user_id = ?", stock, session["user_id"])[0]['SUM(shares)']
        price = lookup(stock["symbol"])["price"]
        total += stock["shares"] * price
    
        data.append({
            "symbol": stock["symbol"],
            "shares": stock["shares"],
            "price": usd(price),
            "total": usd(stock["shares"] * price)
        })
    type(total)
    total = usd(total)
    cash = usd(cash)

    return render_template("index.html", stocks=data, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # validate form
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")
        quote = lookup(symbol)
        
        print(quote)
        try:
            price = float(quote["price"])
        except Exception as e:
            print(e)
            return apology("Please enter a valid symbol")

        # check for valid symbol
        if not symbol:
            return apology("Please enter a valid symbol")
        

        try: 
            shares = int(shares)
        except Exception as e:
            print(e)
            return apology("Value must be an int.")
        
        # check that shares is greater than 1
        if not shares or int(shares) < 1:
            return apology("You must buy at least one or more shares")
        
        # check that user has enough cash
        else:
            cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
            if (price * int(shares)) > cash:
                return apology("You don't have enough cash for this transaction.")
        
        now = datetime.now()
        db.execute(
            "INSERT INTO transactions (symbol, shares, user_id, time) VALUES (?, ?, ?, ?)", 
            symbol, 
            int(shares), 
            session["user_id"],
            now
        )
        
        # update user's cash total
        db.execute("UPDATE users SET cash = cash - ? WHERE id = ?", price * int(shares), session["user_id"])

        return redirect('/')

    else:
        if request.args.get('symbol'):
            symbol = request.args.get('symbol')
        else:
            symbol = None
        return render_template("buy.html", symbol=symbol)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    transactions = db.execute("SELECT symbol, shares, time FROM transactions WHERE user_id = ?", session["user_id"])
    print(transactions)
    for transaction in transactions:
        
        print(transaction["symbol"])
        print(transaction["shares"])
        transaction["value"] = usd(transaction["shares"] * lookup(transaction["symbol"])["price"])
    # transactions["value"] = transactions["shares"] * lookup(transactions["symbol"])
    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        # lookup the price of stock
        symbol = request.form.get("symbol")
        quote = lookup(symbol)

        # make sure symbol is valid
        if not quote:
            return apology(f"{symbol} is not a valid stock symbol")
        else:
            return render_template("quoted.html", symbol=symbol, price=usd(quote['price']))

    
    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # validate form
        # all fields have to be filled in 
        if not request.form.get("username") or not request.form.get("password") or not request.form.get("confirmation"):
            return apology("Missing required form field.")
        # username must be unique
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))
        if len(rows) != 0:
            return apology("Username already taken.")
        
        # passwords must match 
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("Passwords must match")
        
        else:
            # save user to database
            pwhash = generate_password_hash(request.form.get("password"))
            db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", request.form.get("username"), pwhash)

            # log user in 
            id = db.execute("SELECT id FROM users WHERE username = ?", request.form.get("username"))
            session["user_id"] = id[0]["id"]
            return redirect("/")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
        # find out the stocks the user owns
    stocks = db.execute("SELECT symbol, SUM(shares) AS shares FROM transactions WHERE user_id = ? GROUP BY symbol HAVING SUM(shares) > 0;", session["user_id"])
    # stock_symbols = [item["symbol"] for item in stocks]
    stocks_dict = {}
    
    for stock in stocks:
        stocks_dict[stock["symbol"]] = stock["shares"]
    print(stocks_dict)
    if request.method == "POST":
        symbol = request.form.get('symbol')
        try:
            shares = int(request.form.get('shares'))
        except ValueError:
            return apology("Shares must be a positive integer.")
        
        if shares < 1:
            return apology("Shares must be a positive integer.")
        elif not symbol:
            return apology("You must select a stock to sell.")
        elif symbol not in stocks_dict or shares > stocks_dict[symbol]:
            return apology("You don't own enough of that stock.")
        
        print(symbol)
        print(shares)
        # add a negative transaction to transactions
        now = datetime.now()
        db.execute(
            "INSERT INTO transactions (symbol, shares, user_id, time) VALUES (?, ?, ?, ?)", 
            symbol, 
            shares*-1, 
            session["user_id"],
            now
        )

        # add cash to users account
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?", (lookup(symbol)["price"])*shares, session["user_id"])
        return redirect("/")
    else:
        return render_template("sell.html", stocks=stocks)
