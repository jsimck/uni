module MyIndex where

import           Data.Char
import           Data.List

whitespace = [' ', '\t', ',', '.']
text = "In computer science,\tfunctional programming is a programming\n paradigm that treats computation\tas\tthe evaluation of mathematical functions\n and avoids state and mutable data.\tIt emphasizes the application of\n functions, in contrast to\n the imperative\n programming style, which emphasizes changes in state."

copyNtimes :: String -> Int -> String
copyNtimes xs n = concat [rep x | x <- xs] where
    rep x = [x | _ <- [1..n]]

trim :: String -> String
trim [] = []
trim (x:xs) | x `elem` whitespace = ' ' : trim xs
            | otherwise = toLower x : trim xs

clearInvalid :: [[String]] -> Int -> [[String]]
clearInvalid [] _ = []
clearInvalid (x:xs) n = validate x n : clearInvalid xs n where
    validate [] _ = []
    validate (y:ys) m | length y >= n = y : validate ys m
                      | otherwise = validate ys m

removeEmpty :: [[String]] -> [[String]]
removeEmpty [] = []
removeEmpty (x:xs) | null x = removeEmpty xs
                   | otherwise = x : removeEmpty xs

split :: String -> Int -> [[String]]
split x n = removeEmpty(clearInvalid (map words (lines (trim x))) n)

findUnique :: [[String]] -> [String]
findUnique [] = []
findUnique s = fillIndex (concat s) [] where
    fillIndex [] x     = x
    fillIndex (x:xs) i | x `elem` i = fillIndex xs i
                       | otherwise = fillIndex xs (i ++ [x])

occurences :: String -> Int -> [[String]] -> [String]
occurences _ _ [] = []
occurences s n (x:xs) = findOccurence s n x : occurences s (n + 1) xs where
   findOccurence _ _ [] = []
   findOccurence r m (y:ys) | r == y = show m
                            | otherwise = findOccurence r m ys

makeIndex :: [[String]] -> [String] -> [String]
makeIndex _ []      = []
makeIndex db (i:is) = (i ++ copyNtimes " " (18 - length i) ++ intercalate ", " (filter (not . null) (occurences i 1 db))) : makeIndex db is

capitalizeIndex :: [String] -> [String]
capitalizeIndex = map (\ x -> toUpper (head x) : tail x)

groupIndex :: [String] -> Char -> [String]
groupIndex [] _ = []
groupIndex (x:xs) c | head x == c = x : groupIndex xs c
                    | otherwise = ("\n- " ++ [head x] ++ " - \n" ++ x) : groupIndex xs (head x)

generateIndex :: [[String]] -> [String]
generateIndex xs = groupIndex (capitalizeIndex (sort (makeIndex xs (findUnique xs)))) 'a'

pp :: [String] -> IO()
pp = putStr . unlines

-- pp (generateIndex (split text 4))
