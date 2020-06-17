SELECT DISTINCT name FROM ratings
JOIN movies ON ratings.movie_id = movies.id
JOIN stars ON ratings.movie_id = stars.movie_id
JOIN people ON person_id = people.id
WHERE movies.year = 2004 ORDER BY birth