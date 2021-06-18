import Data.Char

calcLuhnChecksum :: Int -> Int
calcLuhnChecksum n = (sum $ oddsn ++ map (\d -> sum $ digits $ d * 2) evensn) * 9 `mod` 10
  where oddsn = odds $ digits n
        evensn = evens $ digits n
        odds (x:y:xs) = x : odds xs
        odds (x:[])   = x : []
        odds _ = []
        evens (x:y:xs) = y : evens xs
        evens _ = []
        digits :: Int -> [Int]
        digits = map ((subtract zero) . ord) . show
          where zero = ord '0'
        
        
