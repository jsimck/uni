module MyStrings where

lineLength = 30
whitespace = ['\n', ' ', '\t']
text = "In computer science, \tfunctional programming is a programming  paradigm that treats computation \tas \tthe evaluation of mathematical functions and avoids state and mutable data. \tIt emphasizes the application of functions, in contrast to the imperative \tprogramming style, which emphasizes changes in state."

clearWhitespace :: String -> String
clearWhitespace [] = []
clearWhitespace (x:xs) | x == '\n' || x == '\t' = clearWhitespace xs
                       | otherwise = x : clearWhitespace xs

takeRNaive :: Int -> [a] -> [a]
takeRNaive n = reverse . take n . reverse

lineLen :: String -> Int
lineLen (_:xs) = 1 + lineLen xs
lineLen []     = 0

maxLen :: [String] -> Int -> Int
maxLen [] x = x
maxLen (x:xs) y | lineLen x > y = maxLen xs (lineLen x)
                | otherwise = maxLen xs y

split :: String -> Int -> [String]
split [] _ = []
split x c | c >= lineLength && head x `elem` whitespace = take c x : split (takeRNaive c)
               | otherwise = [x] : concat (split xs (c + 1))

justify :: String -> Int -> String
justify [] _ = []
justify (x:xs) c | x == ' ' = x : x : justify xs (c - 1)
                 | otherwise = x : justify xs c

j :: [String] -> [String]
j xs = [justify x (maxLen xs 0 - lineLen x) | x <- xs]

pp :: [String] -> IO()
pp = putStr . concat
