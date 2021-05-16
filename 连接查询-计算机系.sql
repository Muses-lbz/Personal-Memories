
SELECT Student.S_NO,S_NAME,CLASS_NAME,Course.COURSE_NO,COURSE_NAME,SCORE
FROM Student,Class,Course,Choice
WHERE (Course.COURSE_NO=Choice.COURSE_NO)
AND (Class.CLASS_NO=Student.CLASS_NO)
AND (Student.S_NO=Choice.S_NO)
AND CLASS_DEPT='计算机系'
