module Loader (parseMachine, Action(L, S, R)) where
import           Data.List.Split
import           System.Environment
import           System.IO

-- Type definitions
data Action = L | S | R deriving (Show)
type State = String
type Rule = (State, Char, State, Char, Action)
type Rules = [Rule]
type Machine = (State, Rules)

-- Parser
clear :: [String] -> [String]
clear [] = []
clear (x:xs) | x == "\n" = clear xs
             | otherwise = clearRule x : clear xs where
                 clearRule [] = []
                 clearRule (r:rs) | r `elem` [' ', '\t', '\n', ')', '('] = clearRule rs
                                  | otherwise = r : clearRule rs

removeNulls :: [String] -> [String]
removeNulls = filter (not . null)

parseRules :: [String] -> Rules
parseRules [] = []
parseRules (x:xs) = parseRule (splitOn "," x) : parseRules xs where
    parseRule rs = (rs!!0, head (rs!!1), rs!!2, head (rs!!3), parseAction (head (rs!!4))) where
        parseAction r | r == '>' = R
                      | r == '<' = L
                      | otherwise = S

parseMachine :: String -> Machine
parseMachine x = (head l, parseRules (tail l)) where
    l = removeNulls (clear (lines x))
