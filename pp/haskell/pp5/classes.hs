module MyClasses where

data Expr = Num Int
    | Add Expr Expr
    | Sub Expr Expr
    | Mul Expr Expr
    | Div Expr Expr
    | Var Char
    deriving (Eq)

-- 4 * 5 + 8 * 2
e1 = Mul (Add (Num 4) (Num 5)) (Add (Num 8) (Num 2))
e2 = Add (Mul (Num 2) (Var 'x')) (Add (Var 'x') (Var 'x'))

eval :: Expr -> Int
eval (Num x) = x
eval (Add x y) = eval x + eval y
eval (Sub x y) = eval x - eval y
eval (Mul x y) = eval x * eval y
eval (Div x y) = eval x `div` eval y

showExpr :: Expr -> String
showExpr e = showExpr' e False where
    showExpr' (Var x)   _ = [x]
    showExpr' (Num x)   _ = show x
    showExpr' (Add x y) f = if f then "(" ++ showExpr' x False ++ " + " ++ showExpr' y False ++ ")"
                            else showExpr' x False ++ " + " ++ showExpr' y False
    showExpr' (Sub x y) f = if f then "(" ++ showExpr' x False ++ " - " ++ showExpr' y False ++ ")"
                            else showExpr' x False ++ " + " ++ showExpr' y False
    showExpr' (Mul x y) _ = showExpr' x True ++ " * " ++ showExpr' y True
    showExpr' (Div x y) _ = showExpr' x True ++ " / " ++ showExpr' y True
