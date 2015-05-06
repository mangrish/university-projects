/*
**  433-351 Project 2 - Hotel bookings database
**  2001 - Semester 1
**
**  SQL INSERT statements for Ingres
**
**  Database:  hotel
**  Requires:  - Project 1 database already exists
*/  

/*
**  Insert into Company table
*/
INSERT INTO company VALUES ('Telstra', 'Melbourne');
INSERT INTO company VALUES ('Optus', 'Melbourne');
INSERT INTO company VALUES ('Microsoft', 'Seattle');
INSERT INTO company VALUES ('LindaLand', 'Melbourne');
INSERT INTO company VALUES ('LindaLand', 'Sydney');
INSERT INTO company VALUES ('Qantas', 'Brisbane');
INSERT INTO company VALUES ('Coles Myer', 'Sydney');
INSERT INTO company VALUES ('Apple Inc.', 'San Jose');
\g

/*
**  Insert into Owners table
*/
INSERT INTO owners VALUES ('Telstra', 'Melbourne', 'Regent Hotel','Sydney', 0.5); 
INSERT INTO owners VALUES ('Optus', 'Melbourne', 'Queens Hotel', 'London', 0.1);
INSERT INTO owners VALUES ('Microsoft', 'Seattle', 'Optus', 'Melbourne', 0.1);
INSERT INTO owners VALUES ('LindaLand', 'Melbourne', 'Microsoft', 'Seattle', 1.0);
INSERT INTO owners VALUES ('LindaLand', 'Melbourne','Telstra', 'Melbourne', .623);
INSERT INTO owners VALUES ('Coles Myer', 'Sydney', 'Ritz Hotel', 'New York', .237);
INSERT INTO owners VALUES ('Qantas', 'Brisbane', 'Coles Myer', 'Sydney', .237);
INSERT INTO owners VALUES ('Qantas', 'Brisbane', 'Ritz Motel', 'New York', .714);
INSERT INTO owners VALUES ('LindaLand', 'Sydney', 'L`Hotel de Paris', 'Paris', .477);
INSERT INTO owners VALUES ('Apple Inc.', 'San Jose', 'LindaLand', 'Sydney', .172);
\g

