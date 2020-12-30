module ComplexLists where

type Pic = [String]

obr :: Pic
obr = ["....#....", "...###...", "..#.#.#..", ".#..#..#.", "....#....", "....#....", "....#####"]

pp :: Pic -> IO()
-- pp = putStr . concat . map (++"\n")
-- pp = putStr . concatMap (++"\n")
pp = putStr . unlines

-- Transformation functions
flipH :: Pic -> Pic
flipH = reverse

flipV :: Pic -> Pic
flipV = map reverse

above :: Pic -> Pic -> Pic
above os ps = os ++ ps

sideBySide :: Pic -> Pic -> Pic
sideBySide (o:os) (p:ps) = (o ++ p) : sideBySide os ps
sideBySide _ _ = []

column :: String -> Pic
column os = [[x] | x <- reverse os]

rotateL :: Pic -> Pic
rotateL [x] = reverse(column x)
rotateL (x:xs) = reverse(column x) `sideBySide` rotateL xs

rotateR :: Pic -> Pic
rotateR [x] = reverse(column x)
rotateR (x:xs) = rotateR xs `sideBySide` reverse(column x)

copyNtimes :: [a] -> Int -> [a]
copyNtimes xs n = concat [rep x | x <- xs] where
    rep x = [x | _ <- [1..n]]

scale :: Pic -> Int -> Pic
scale xs n = copyNtimes [copyNtimes x n | x <- xs] n
