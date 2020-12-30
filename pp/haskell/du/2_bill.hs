module Bill where
import           Data.List
import           Text.Printf

type Name = String
type Price  = Int
type BarCode = Int
type Database = [(BarCode, Name, Price)]
type TillType = [BarCode]
type BillType = [(Name, Price)]

-- Db + Till
base = [(110, "Cerealie", 39),(666, "Maslo", 63),(111, "Kureci maso 500g (velmi dlouhy nazev)", 99),(112, "Nescafe",99),(113, "Slepici bujon", 12)]

-- Functions
copyNtimes :: [a] -> Int -> [a]
copyNtimes xs n = concat [rep x | x <- xs] where
    rep x = [x | _ <- [1..n]]

lineLen :: String -> Int
lineLen (_:xs) = 1 + lineLen xs
lineLen []     = 0

maxLen :: [String] -> Int -> Int
maxLen [] x = x
maxLen (x:xs) y | lineLen x > y = maxLen xs (lineLen x)
                | otherwise = maxLen xs y

numOccurence :: Name -> BillType -> Int
numOccurence x b = length [[name] | (name, _) <- b, name == x]

line :: Int -> String
line l = "|" ++ copyNtimes "-" l ++ "|\n"

emptyLine :: Int -> String
emptyLine l = "|" ++ copyNtimes " " l ++ "|\n"

header :: Int -> String
header l = line l ++ emptyLine l ++ "|" ++ copyNtimes " " ((l `div` 2) - 2) ++ "Ucet" ++ copyNtimes " " ((l `div` 2) - 2) ++ "|\n" ++ emptyLine l ++ line l

stripName :: String -> Int -> String
stripName s x | lineLen s <= x = s
              | otherwise = take x s ++ "..."

formatLine :: (Name, Price) -> BillType -> String
formatLine (n, p) b = "| " ++ stripName n 16 ++ copyNtimes " " (30 - lineLen (stripName n 16 ++ show p) - lineLen (show (numOccurence n b))) ++ show (numOccurence n b) ++ "x " ++ show p ++ ",- |\n"

formatBill :: BillType -> String
formatBill bill = header 36 ++ concat [formatLine (name, price) bill | (name, price) <- nub bill] ++ line 36 ++ celkem bill ++ line 36 where
    celkem b = "| Celkem" ++ copyNtimes " " (26 - lineLen (show (sum [price | (_, price) <- b]))) ++ show (sum [price | (_, price) <- b]) ++ ",- |\n"

makeBill :: TillType -> BillType
makeBill xs = concat [item code | code <- xs] where
    item c = [(name, price) | (bar, name, price) <- base, c == bar]

produceBill :: TillType -> String
produceBill = formatBill . makeBill

pp :: String -> IO()
pp = putStr

-- pp (produceBill [110, 666, 666, 666, 112, 113, 110, 110, 111])
