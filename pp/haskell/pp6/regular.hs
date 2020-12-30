module RegularExpression where

data Reg = Hod Char
        | Iter Reg
        | Zret Reg Reg
        | Sjed Reg Reg

type Prechod = (Char, Int, Int)
type Automat = ([Char], Int, Int, [Int], [Prechod])

reg1 = Zret (Zret (Iter (Sjed (Hod 'a') (Hod 'b'))) (Hod 'a')) (Hod 'b')
reg2 = Iter (Hod 'a')

showReg :: Reg -> String
showReg (Hod x)    = [x]
showReg (Iter x)   = "(" ++ showReg x ++ ")*"
showReg (Zret x y) = showReg x ++ "." ++ showReg y
showReg (Sjed x y) = "(" ++ showReg x ++ "+" ++ showReg y ++ ")"

instance Show Reg where
    show x = showReg x

posun :: Automat -> Int -> Automat
posun (a, q, s, f, p) x = (a, q + x, s + s, map (+x) f, [(p1, p2+x, p3+x) | (p1, p2, p3) <- p])

unique :: [Char] -> [Char] -> [Char]
unique :: xs ys = xs ++ [y | y <- ys, notElem y xs]

preved :: Reg -> Automat
preved (Hod x)  = ([x], 2, 0, [1], [(x,0,1)])
preved (Iter x) = let (a, q, s, f, p) = posun (preved x) 1
                  in (a, q, 0, [0], ('E',0,s):p ++ [('E', f1, 0) | f1 <- f])
-- preved (Zret x y) = preved x ++ "." ++ preved y
preved (Sjed x y) = let (xa, xq, xs, xf, xp) = posun (preved x) 1
                    in let (ya, yq, ys, yf, yp) = posun (preved y) xq
                    in (unique ya xa, yq, 0, xf ++ yf, xp ++ yp ++ )
