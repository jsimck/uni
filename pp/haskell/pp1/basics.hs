-- Fibonaci
fib :: Int -> Int
fib 0 = 0
fib 1 = 1
fib n = fib (n-1) + fib (n-2)

-- Delka seznamu
lstLen :: [a] -> Int
lstLen [] = 0
lstLen (x:xs) = 1 + lstLen xs

-- Nejmensi spolecny delitel
nsd :: Int -> Int -> Int
nsd a b
    | a==b = a
    | a>b  = nsd (a-b) b
    | a<b  = nsd a (b-a)

-- Spoji 2 seznamy do jednoho zasebou
merge :: [a] -> [a] -> [a]
merge [] x = x
merge (x:xs) y = x:(merge xs y)

-- Udela z dvou seznamů dvojice
doubles :: [a] -> [b] -> [(a,b)]
doubles [] _ = []
doubles _ [] = []
doubles (x:xs) (y:ys) = (x, y):doubles xs ys

-- Otoci seznam
swap :: [a] -> [a]
swap [] = []
swap (x:xs) = swap xs ++ [x]

-- Jina varianta otoceni
swap2 x = swap2Tmp x []
swap2Tmp [] ys = ys
swap2Tmp (x:xs) ys = swap2Tmp xs (x:ys)
