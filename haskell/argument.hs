module Argument
( decomposeLine
, split
, strip
, nextToken
, lexExpr
) where

import Data.List  -- break
import Data.Bits  -- xor
import Data.String
import Data.Char

data LineType = Error | Premise | Conclusion | Imply deriving (Show, Eq)
data ExprType = Not | And | Xor | Or | Ident | LParen | RParen deriving (Show, Eq)
data ExprNode = ExprNode {
  children :: [ExprNode],
  operator :: [Bool] -> Bool
} deriving (Show)

split :: (Eq a) => a -> [a] -> ([a], [a])
split c s = let (a,b) = break (==c) s in (a, tail b)

strip :: String -> String
strip s = dropWhile isSpace $ dropWhileEnd isSpace s

decomposeLine :: String -> (LineType, String, String)
decomposeLine ln = let (a,b) = (\(x,y) -> (strip x, strip y)) $ split ':' ln
  in let a1:a2:_ = words a in case a1 of
    "premise" -> (Premise, a2, b)
    "conclusion" -> (Conclusion, a2, b)
    "imply" -> (Imply, a2, b)
    otherwise -> (Error, a2, b)

getType :: String -> ExprType
getType sr
  | s == "~" = Not
  | s == "&" = And
  | s == "^" = Xor
  | s == "|" = Or
  | s == "(" = LParen
  | s == ")" = RParen
  | otherwise = Ident
  where
    s = strip sr

nextToken :: String -> (ExprType,String)
nextToken s@(h:_) = (t, if t /= Ident then [h] else takeWhile (\c -> getType [c] == Ident) (head $ words s)) where t = getType [h]

lexExpr :: String -> [(ExprType,String)]
lexExpr [] = []
lexExpr s@(x:_) = if isSpace x then lexExpr (tail s) else
  let t = nextToken s in [t] ++ lexExpr (drop (length $ snd t) s)

buildExprTree :: [(ExprType,String)] -> (String -> Bool) -> ExprNode
