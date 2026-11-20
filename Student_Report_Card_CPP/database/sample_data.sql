PRAGMA foreign_keys = ON;

INSERT OR IGNORE INTO students(name, roll_number, department, semester) VALUES
('Arjun Kumar', 'STU001', 'CSE', 3),
('Priya Sharma', 'STU002', 'CSE', 3),
('Rahul Reddy', 'STU003', 'ECE', 3);

INSERT OR REPLACE INTO marks(student_id, subject, mark)
SELECT id, 'Mathematics', 92 FROM students WHERE roll_number = 'STU001';
INSERT OR REPLACE INTO marks(student_id, subject, mark)
SELECT id, 'Programming', 88 FROM students WHERE roll_number = 'STU001';
INSERT OR REPLACE INTO marks(student_id, subject, mark)
SELECT id, 'Digital Electronics', 95 FROM students WHERE roll_number = 'STU001';

INSERT OR REPLACE INTO marks(student_id, subject, mark)
SELECT id, 'Mathematics', 81 FROM students WHERE roll_number = 'STU002';
INSERT OR REPLACE INTO marks(student_id, subject, mark)
SELECT id, 'Programming', 90 FROM students WHERE roll_number = 'STU002';
