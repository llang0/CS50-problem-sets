-- all people who have directed a movie with a rating of 9.0 or higher
-- one column with directors name

SELECT name FROM people WHERE id IN(
    SELECT person_id FROM directors WHERE movie_id IN(
        SELECT movie_id FROM ratings WHERE rating >= 9.0
    )
);
