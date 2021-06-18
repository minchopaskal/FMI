import Data.Char
import Data.List

-- Ex.1
reverseOrdStuff :: Int -> Int
reverseOrdStuff = readInt . reverse . show . last . sort . 
  map readInt . filter descending . filter (not . null) . tails . show
  where readInt :: String -> Int
        readInt = read
        descending (x:[]) = True
        descending (x:y:xs) = ord x > ord y && descending (y:xs)

-- Ex. 2
sumUnique :: [[Int]] -> Int
sumUnique = sum . concat . map delNonUnique
  where delNonUnique [] = []
        delNonUnique (x:xs)
          | elem x xs = delNonUnique (filter (not . (==x)) xs)
          | otherwise = x : delNonUnique xs

-- Ex. 3
type Product = (String,Double)
type StoreAvailability = [Product]

-- a)
closestToAverage :: StoreAvailability -> String
closestToAverage xs = let l = fromIntegral $ length xs in
  let avg = (/l) . sum . map (\(_, price) -> price) $ xs
  in snd . head . sort . map (\(s, d) -> (abs $ d - avg, s)) $ xs

-- b)
cheaperAlternative :: StoreAvailability -> Int
cheaperAlternative [] = 0
cheaperAlternative (x:xs) =
  if diff xs x
  then 1 + cheaperAlternative (filter (not . (==fst x) . fst) xs)
  else cheaperAlternative xs
  where diff [] _ = False
        diff (x:xs) el
          | fst x == fst el   = snd x > snd el || snd x < snd el || diff xs el
          | otherwise = diff xs el

-- Ex. 4
type Point = (Double, Double, Double) 

distance :: Point -> Point -> Double
distance (x1, y1, z1) (x2, y2, z2) = (x1 - x2) ^ 2 + (y1 - y2) ^ 2 + (z1 - z2) ^ 2

minDistance :: [(Double, Double, Double)] -> Double
minDistance ps = let distances = [ distance p1 p2 | p1 <- ps, p2 <- filter (not . (==p1)) ps]
                 in  head . sort $ distances

-- Ex. 5
maximize :: (Ord a, Num a) => [(a -> a)] -> (a -> a)
maximize fs x = snd . last . sort . map (\f -> (abs $ f x, f x)) $ fs

-- Ex. 6
inverseFun :: (Int -> Int) -> (Int -> Int) -> Int -> Int -> Bool
inverseFun f g a b = null . filter (\x -> fst x /= snd x) . map (\x -> (f .g $ x, x)) $ [a..b]

-- Ex. 7
data BTree a = Empty | Node a (BTree a) (BTree a)
  deriving (Show, Read)

-- getChildren function would help to build the list in sorted order
getLevels :: BTree a -> [(a, Int)]
getLevels Empty = []
getLevels bt = getLevelsIndexed bt 0
  where getLevelsIndexed Empty _ = []
        getLevelsIndexed (Node n l r) level = [(n, level)]
                                              ++ getLevelsIndexed l (level + 1)
                                              ++ getLevelsIndexed r (level + 1)

inorder :: BTree a -> [a]
inorder Empty = []
inorder (Node n l r) = inorder l ++ [n] ++ inorder r

mirrorBst :: BTree a -> BTree a
mirrorBst Empty = Empty
mirrorBst (Node n l r) = Node n (mirrorBst r) (mirrorBst l)
                        
