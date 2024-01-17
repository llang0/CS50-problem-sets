-- All people who have starred in a movie with Kevin Bacon
-- excluding Kevin Bacon himself


SELECT name FROM people JOIN stars ON stars.person_id = people.id WHERE stars.movie_id IN (
    SELECT movie_id FROM stars WHERE person_id IN(
        SELECT id FROM people WHERE name = 'Kevin Bacon' AND birth = 1958
    )
) AND people.name != 'Kevin Bacon';
