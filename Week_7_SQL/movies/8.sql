SELECT name FROM people WHERE id IN (
    SELECT person_id FROM stars WHERE movie_id IN(
        SELECT id FROM movies WHERE title = 'Toy Story'
    )
);


-- SELECT people.name
-- FROM people JOIN stars JOIN movies ON people.id = stars.person_id AND movies.id = stars.movie_id
-- WHERE title = 'Toy Story';
