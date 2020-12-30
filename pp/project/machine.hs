import           Loader

-- Type definitions
data Tape = Tape String Char String Int

instance Show Tape where
    show (Tape ls h rs n) = left ls ++ "[" ++ [h] ++ "]" ++ right (rs ++ repeat '.') where
        left l = reverse (take (n `div` 2) (l ++ repeat '.'))
        right r = take (n `div` 2) (r ++ repeat '.')

type State = String
type Rule = (State, Char, State, Char, Action)
type Rules = [Rule]
type Machine = (State, Rules)

-- Functions for rule access
stateOfR :: Rule -> State
stateOfR (x, _, _, _, _) = x

headOfR :: Rule -> Char
headOfR (_, x, _, _, _) = x

nextStateOfR :: Rule -> State
nextStateOfR (_, _, x, _, _) = x

nextHeadOfR :: Rule -> Char
nextHeadOfR (_, _, _, x, _) = x

actionOfR :: Rule -> Action
actionOfR (_, _, _, _, x) = x

-- Functions for tape manipulation
headT :: Tape -> Char
headT (Tape _ x _ _) = x

lsT :: Tape -> String
lsT (Tape x _ _ _) = x

rsT :: Tape -> String
rsT (Tape _ _ x _) = x

lengthT :: Tape -> Int
lengthT (Tape _ _ _ x) = x

prepareT :: String -> Int -> Tape
prepareT x = Tape ".." (head x) (tail x ++ "..")

moveT :: Tape -> Char -> Action -> Tape
moveT (Tape (l:ls) _ (r:rs) n) x L = Tape ls l (x:r:rs) n
moveT (Tape ls _ rs n) x         S = Tape ls x rs n
moveT (Tape (l:ls) _ (r:rs) n) x R = Tape (x:l:ls) r rs n

-- Apply one rule on tape with current state
changeT :: Tape -> State -> Rules -> (Tape, State, Bool)
changeT t s []     = (t, s, True)
changeT t s (r:rs) | cs == s && th == rh = (moveT t rnh ra, rns, False)
                   | otherwise = changeT t s rs where
                        th = headT t
                        cs = stateOfR r
                        rh = headOfR r
                        rnh = nextHeadOfR r
                        ra = actionOfR r
                        rns = nextStateOfR r

-- Helper functions for accessing machine
rulesOfM :: Machine -> Rules
rulesOfM (_, x) = x

initialStateOfM :: Machine -> State
initialStateOfM (x, _) = x

-- Turing machine functions
compute :: Tape -> State -> Rules -> Bool -> [Tape] -> [Tape]
compute t s rs e ts | e == True = reverse ts
                    | otherwise = let (nT, nS, nE) = changeT t s rs
                                  in compute nT nS rs nE (t:ts)

run :: Machine -> Tape -> [Tape]
run m t = compute t "A" (rulesOfM m) False []

printT :: [Tape] -> IO()
printT x = putStr (unlines (showT x)) where
    showT = map show

main :: IO ()
main = do putStrLn "\n=========================="
          putStrLn "=      Turing Machine    ="
          putStrLn "=========================="
          putStrLn "\nPath to turing machine:"
          fp <- getLine
          contents <- readFile fp
          putStrLn "\nSet content of tape:"
          tp <- getLine
          putStrLn ""
          putStrLn "Set visible tape size:"
          tpsz <- readLn
          putStrLn ""
          let result = run (parseMachine contents) (prepareT tp tpsz)
          putStrLn "Result:"
          printT result
          putStrLn ("\nNumber of steps: " ++ show (length result) ++ "\n")
