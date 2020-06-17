SELECT rating, title FROM movies JOIN ratings ON movies.id = ratings.movie_id
WHERE rating != '\\N' AND movies.year = 2010
ORDER BY rating DESC, title
