doubleMe :: Int -> Int
doubleMe x = x * x

doubleUs x y = doubleMe x + doubleMe y

doubleSmallNumber x = if x > 100
    then x
    else x * 2

doubleSmallNumber2 x = (if x > 100 then x else x * 2) + 1

boombangs xs = [if x < 10 then "BOOM" else "BANGS" | x <- xs, odd x]