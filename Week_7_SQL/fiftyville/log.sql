-- Query 1 Look at the crime scene reports at the given time and place

SELECT description
FROM crime_scene_reports
WHERE month = 7
AND day = 28
AND street = 'Humphrey Street';

-- Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery.
-- Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.


-- Query 2 Look at interviews on given day

SELECT name, id, transcript
FROM interviews
WHERE month = 7
AND day = 28;

-- only 3 interviews contain relevant information

-- | Ruth    | 161 | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away.
-- If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.

-- | Eugene  | 162 | I don't know the thief's name, but it was someone I recognized.
-- Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.

-- | Raymond | 163 | As the thief was leaving the bakery, they called someone who talked to them for less than a minute.
-- In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow.
-- The thief then asked the person on the other end of the phone to purchase the flight ticket. |


-- Query 3, get a list of phone calls made that day that last less that one minute

SELECT * FROM phone_calls
WHERE month = 7
AND day = 28
AND duration <= 60;


-- Query 4, anyone who placed a call is suspect, create a table of suspects by joining the above query with the people table

-- CREATE TABLE suspects AS
-- SELECT people.id, name, phone_number, passport_number, license_plate
-- FROM people JOIN phone_calls ON people.phone_number = phone_calls.caller
-- WHERE month = 7
-- AND day = 28
-- AND duration <= 60;

SELECT * FROM suspects;

-- Query 5 Look at atm withdrawals at Leggertt Street on the day of the theft

SELECT * FROM atm_transactions
WHERE month = 7
AND day = 28
AND atm_location = 'Leggett Street'
AND transaction_type = 'withdraw';



-- QUERY 6 Join atm_transactions with bank accounts, and see if the person id matches any suspects to
-- see if anyone on the suspect list was withdrawing money from Leggett Street ATM.

SELECT name
FROM suspects JOIN bank_accounts ON person_id = id
WHERE account_number IN(
    SELECT account_number FROM atm_transactions
    WHERE month = 7
    AND day = 28
    AND atm_location = 'Leggett Street'
    AND transaction_type = 'withdraw'
);


-- QUERY 7 Drop everyone except the above people from the suspects table and check results

-- DELETE FROM suspects
-- WHERE id NOT IN (
--     SELECT person_id FROM bank_accounts
--     WHERE account_number IN(
--         SELECT account_number FROM atm_transactions
--         WHERE month = 7
--         AND day = 28
--         AND atm_location = 'Leggett Street'
--         AND transaction_type = 'withdraw'
--     )
-- );

SELECT * FROM suspects;


-- Query 8 Check bakery security logs between 10:05 - 10:25, looking for license plate numbers that appear on the suspects table

SELECT name, bakery_security_logs.license_plate
FROM bakery_security_logs
JOIN people ON bakery_security_logs.license_plate = people.license_plate
WHERE month = 7
AND day = 28
AND hour = 10
AND minute BETWEEN 05 AND 25
AND bakery_security_logs.license_plate IN (
    SELECT license_plate FROM suspects
);



-- Query 9 Bruce and Diana are looking very suspicious. They both withdrew money from the Leggett Street ATM
-- and were caught on security footage leaving the bakery.

-- Raymond overheard the theif saying they got on the first flight out of Fiftyville the following day.

-- Find the first flight leaving Fiftyville the following day

SELECT * FROM flights
WHERE month = 7
AND day = 29
AND origin_airport_id IN (
    SELECT id FROM airports WHERE city = 'Fiftyville'
)
ORDER BY hour
LIMIT 1;

-- This shows that flight 36 is the first flight leaving Fiftyville the day after the crime

-- Query 10 See if any passengers of flight 36 appear on suspects list

SELECT name
FROM suspects JOIN passengers
ON suspects.passport_number = passengers.passport_number
WHERE flight_id = 36;

-- This shows that Bruce, Taylor, and Kenney were all on the first flight out of fiftyville the next day
-- Since Bruce was also seen leaving the Bakery and withdrawing money,
-- He is likely the theif

-- Query 11 Find the accomplish by seeing who Bruce called. I can see he called (375) 555-8161, so I'll
-- just check who that number belongs to

SELECT name
FROM people
WHERE phone_number = '(375) 555-8161';

-- It looks like Bruce's accomplice is Robin

-- Query 12, Find out what city Bruce escaped to. I already know the flight id is 36
SELECT city
FROM airports
WHERE id IN (
    SELECT destination_airport_id
    FROM flights
    WHERE id = 36
);

-- So Bruce escaped to New York City, and his accomplice is Robin
