-- Chadwick Boseman's top 5 movies

SELECT title
FROM movies JOIN ratings ON ratings.movie_id = movies.id
WHERE id IN (
    SELECT stars.movie_id FROM stars WHERE stars.person_id IN(
        SELECT id FROM people WHERE name = 'Chadwick Boseman'
    )
)
ORDER BY rating DESC
LIMIT 5;
