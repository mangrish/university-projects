/*
**  433-351 Project 2 - Hotel database
**
**  SQL CREATE TABLE statements for Ingres
*/  


CREATE TABLE company
(
  coname VARCHAR(11) NOT NULL,
  cocity VARCHAR(10) NOT NULL,
  PRIMARY KEY (coname, cocity)
)
\g

CREATE TABLE owners
(
  parentname 	VARCHAR(11) NOT NULL,
  parentcity 	VARCHAR(10) NOT NULL,
  childname 	VARCHAR(21) NOT NULL,
  childcity 	VARCHAR(10) NOT NULL,
  fraction      float,
  CONSTRAINT check_fraction CHECK (fraction > 0 AND fraction <= 1.0),
  FOREIGN KEY (parentname, parentcity) REFERENCES company,
  PRIMARY KEY (parentname, parentcity, childname, childcity)
)
\g

