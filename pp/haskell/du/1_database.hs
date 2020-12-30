module Database where

type Person = String
type Book = String
type Database = [(Person, Book)]

-- Return list of books for Person
_books :: Database -> Person -> [Book] -> [Book]
_books [] _ ys = ys
_books ((p,b):ds) x ys | p == x    = _books ds x (ys ++ [b])
                       | otherwise = _books ds x ys

books :: Database -> Person -> [Book]
books ds x = _books ds x []

-- Alternative
books' :: Database -> Person -> [Book]
books' ds p = [book | (book, person) <- ds, person == p]


-- Check if book is in database
borrowed :: Database -> Book -> Bool
borrowed [] _ = False
borrowed ((_,b):ds) x | b == x    = True
                      | otherwise = borrowed ds x


-- Number of books
_numBorrowed :: Database -> Book -> Int -> Int
_numBorrowed [] _ x = x
_numBorrowed ((_,b):ds) x y | b == x    = _numBorrowed ds x (y + 1)
                            | otherwise = _numBorrowed ds x y

numBorrowed :: Database -> Book -> Int
numBorrowed ds b = _numBorrowed ds b 0


-- Adds person and book to db
makeLoan :: Database -> Person -> Book -> Database
makeLoan ds p b = ds ++ [(p, b)]


-- Adds person and book to db
returnLoan :: Database -> Person -> Book -> Database
returnLoan [] _ _ = []
returnLoan ((p, b):ds) x y | p == x && b == y = ds
                           | otherwise        = (p,b) : returnLoan ds x y
