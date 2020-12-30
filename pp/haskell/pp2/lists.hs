-- alternativa ku zipWith
spojs :: (a -> b -> c) -> [a] -> [b] -> [c]
spojs a (x:xs) (y:ys) = a x y : spojs a xs ys
spojs a _ y = []

-- skalarni soucin
sks :: [Int] -> [Int] -> Int
sks (x:xs) (y:ys) = x * y + sks xs ys
sks x y = 0

-- kartesky soucin
ks :: [Int] -> [Int] -> [(Int, Int)]
ks (x:xs) (y:ys) = [(x,y) | x <- x:xs, y <- y:ys]

-- kartesky soucin manualne
ksmTmp x (y:ys) = (x,y) : ksmTmp x ys
ksmTmp x [] = []

ks2 (x:xs) ys = (ksmTmp x ys) ++ ks2 xs ys
ks2 [] _ = []

ks3 x [] = []
ks3 [x] (y:ys) = (x,y) : ks3 [x] ys
ks3 (x:xs) ys = ks3 [x] ys ++ ks3 xs ys

-- nejmensi prvek
nejmensi :: (Ord a) => [a] -> a
nejmensi (x:xs) = tmp x xs where
    tmp mn [] = mn
    tmp mn (x:xs) | mn > x = tmp x xs
                  | otherwise = tmp mn xs

-- nejmensi prvek
minEl :: Ord a => [a] -> a
minEl (x:y:xs) = (if x < y then minEl (x:xs) else minEl (y:xs))
minEl [x] = x

-- delitele generator
delitele x = [y | y <- [1..x], mod x y == 0]
