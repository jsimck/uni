module NSH08 where

data Shape = Circle Float Float Float
           | Rectangle Float Float Float Float

showShape :: Shape -> String
showShape (Circle x y z)    = "() " ++ show x ++ ", " ++ show y ++ ", " ++ show z
showShape (Rectangle x y z w) = "[] " ++ show x ++ ", " ++ show y ++ ", " ++ show z ++ ", " ++ show w

instance Show Shape where
    show x = showShape x
