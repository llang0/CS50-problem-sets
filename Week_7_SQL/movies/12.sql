-- titles of all movies with both Bradley Cooper and Jennifer Lawrence
-- Single column with only movie title
-- all movies with both Bradley Cooper and Jennifer Lawrence

SELECT title FROM movies WHERE id IN (
    SELECT movie_id FROM stars WHERE person_id IN(
        SELECT id FROM people WHERE name = 'Jennifer Lawrence'
    )
) AND id IN (
    SELECT movie_id FROM stars WHERE person_id IN(
        SELECT id from people WHERE name = 'Bradley Cooper'
    )
);
