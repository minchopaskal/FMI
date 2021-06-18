import Data.Char
import Data.Digits
import Data.List
import Data.List.Split

-- Ex. 1
convert x from to = unDigits from (digits to x) 

-- Ex. 2
sumNumbers xs = sum [read x :: Int | x <- (wordsBy (\x -> (not (isDigit x))) xs)]

-- Ex. 3
encode :: [Char] -> [(Char, Int)]
encode = (map (\xs -> (head xs, length xs))) . group

-- Ex. 4
maximize fs x = snd (maximum [(abs (f x), f x) | f <- fs])

