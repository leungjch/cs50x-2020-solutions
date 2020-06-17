SELECT DISTINCT name FROM stars JOIN people ON stars.person_id = people.id
WHERE name != "Kevin Bacon" AND stars.movie_id IN
	(SELECT movie_id FROM movies
	JOIN stars ON movies.id = stars.movie_id
	JOIN people ON stars.person_id = people.id
	WHERE
	people.name = "Kevin Bacon" AND people.birth = 1958)