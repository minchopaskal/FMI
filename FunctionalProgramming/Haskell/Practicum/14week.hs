import Prelude

{- C++ enum -}
-- Сезони
data Season = Spring | Summer | Autumn | Winter
    deriving (Read, Show)

-- Месеци
data Month = January | February | March | April | May | June | July | August | September | October | Novemer | December
    deriving (Read, Show)

{- C++ struct or (tagged) union -}
data Person = Person String Int
-- record syntax:
-- data Person = Person { name :: String, age :: Int }
--     deriving (Read, Show)
data Book = Book { bookTitle :: String, bookPublished :: Int, bookSales :: Int }
    deriving (Read, Show)

data Shape = Circle Double | Square Double 
    deriving (Read, Show)

{- C++ container classes -}
-- Списък от произволни елементи (от тип a)
data List a = Nil | Cons a (List a)
    deriving (Read, Show)

-- Tree a, описващ произволно двоично дърво, чийто стойности са от тип a.
data Tree a = Empty | Node a (Tree a) (Tree a)
    deriving (Read, Show)


{- Примери -}
-- Дълбочина на двоично дърво:
treeDepth :: (Num b, Ord b) => Tree a -> b
treeDepth Empty = 0
treeDepth (Node _ left right) = 1 + max (treeDepth left) (treeDepth right)

-- Брой на листата на двоично дърво:
treeCountLeaves :: (Num b) => Tree a -> b
treeCountLeaves Empty = 0
treeCountLeaves (Node _ Empty Empty) = 1
treeCountLeaves (Node _ left right) = treeCountLeaves left + treeCountLeaves right

-- Сбор на всички стойности на върховете на двоично дърво:
treeSum :: Num a => Tree a -> a
treeSum Empty = 0
treeSum (Node x left right) = x + treeSum left + treeSum right

-- Списък със стойностите на всички вървове на ниво k:
nodesOnLevel :: (Integral b) => Tree a -> b -> [a]
nodesOnLevel Empty _ = []
nodesOnLevel (Node v left right) k
    | k < 0     = []
    | k == 0    = [v]
    | otherwise = nodesOnLevel left (k-1) ++ nodesOnLevel right (k-1)  

leftVal :: (Ord a) =>  Tree a -> a
leftVal (Node _ (Node n _ _)  _) = n

rightVal :: (Ord a) => Tree a -> a
rightVal (Node _ _ (Node n _ _)) = n

{- Задачи -}
{-
Задача 1. Дефинирайте функцията listSpecial tree, която приема двоично дърво tree
и връща списък от тези стойности на върховете на tree, които са равни на сбора на 
стойностите на децата си.

Примери:
    tree11 = (Node 3 
                (Node 1 
                    (Node 1 Empty Empty)
                    Empty)
                (Node 2 
                    (Node 4 Empty Empty)
                    (Node 7 Empty Empty)))
    
    listSpecial tree11 -> [3, 1]
-}
listSpecial :: (Eq a, Num a) => Tree a -> [a]
listSpecial Empty     = []
listSpecial (Node n l r)
  | n == sumChild l r = [n] ++ listSpecial l ++ listSpecial r
  | otherwise         = listSpecial l ++ listSpecial r
  where sumChild Empty (Node n _ _)          = n
        sumChild (Node n _ _) Empty          = n
        sumChild (Node n1 _ _) (Node n2 _ _) = n1 + n2
        sumChild _ _                         = 0


{-
Задача 2. Дефинирайте функцията areMirrorImages tree1 tree2, която приема две двойчни
дървета tree1 и tree2 и връща дали те са огледални образи едно на друго.

Примери:
    tree21 = (Node 4 
                (Node 3 
                    (Node 1 Empty Empty)
                    (Node 2 Empty Empty))
                (Node 5 
                    Empty
                    (Node 6 Empty Empty)))
    tree22 = (Node 4 
                (Node 5 
                    (Node 6 Empty Empty)
                    Empty)
                (Node 3 
                    (Node 2 Empty Empty)
                    (Node 1 Empty Empty)))
    tree23 = (Node 4 
                (Node 5 
                    (Node 6 Empty Empty)
                    Empty)
                (Node 3 
                    (Node 1 Empty Empty)
                    (Node 2 Empty Empty)))

    areMirrorImages tree21 tree22 -> True
    areMirrorImages tree21 tree23 -> False    
-}
areMirrorImages :: (Eq a) => Tree a -> Tree a -> Bool
areMirrorImages Empty Empty        = True
areMirrorImages (Node _ _ _) Empty = False
areMirrorImages Empty (Node _ _ _) = False
areMirrorImages (Node n1 l1 r1) (Node n2 l2 r2)
     = n1 == n2
    && areMirrorImages l1 r2
    && areMirrorImages l2 r1


{-
Пордредени (сортирани) двоични дървета: 
 - Всеки вътрешен възел складира ключ (и по избор и стойност свъзрана с ключа).
 - Ключът във всеки възел трябва да е по-голям от всички ключове пазени влявото поддърво 
   и по-малък от всички ключове пазени в дясното поддърво.
-}

{-
Задача 3. Дефинирайте функцията isBST tree, която приема двоично дърво tree и връща
дали е подредено.

Примери:
    tree31 = (Node 4 
                (Node 2 
                    (Node 1 Empty Empty)
                    (Node 3 Empty Empty))
                (Node 5 
                    Empty
                    (Node 6 Empty Empty)))
    tree32 = (Node 4 
                (Node 2 
                    (Node 1 Empty Empty)
                    (Node 3 Empty Empty))
                (Node 5 
                    (Node 6 Empty Empty)
                    Empty))

    isBST tree31 -> True
    isBST tree32 -> False
-}
isBST :: (Ord a) => Tree a -> Bool
isBST Empty = True
isBST (Node n l r)
   | isBigger l && isSmaller r = isBST l && isBST r
   | otherwise                       = False
   where
     isBigger Empty         = True
     isBigger (Node m _ _)  = n > m
     isSmaller Empty        = True
     isSmaller (Node m _ _) = n < m

{-
Задача 4. Дефнирайте функцията bstToList tree, която приема подредено 
двоично дърво tree и връща списък с ключовете на дървото подредени във
възходящ ред.

Примери: 
    tree   = (Node 4 
                (Node 2 
                    (Node 1 Empty Empty)
                    (Node 3 Empty Empty))
                (Node 5 
                    Empty
                    (Node 6 Empty Empty)))

    bstToList tree -> [1, 2, 3, 4, 5, 6]
-}
bstToList :: (Ord a) => Tree a -> [a]
bstToList = sort . nodes
  where
    sort [] = []
    sort (x:xs) = sort [y | y <- xs, y <= x] ++ [x] ++ sort [y | y <- xs, y > x]
    nodes Empty = []
    nodes (Node n l r) = [n] ++ nodes l ++ nodes r

{-
Задача 5. Дефинирайте функцията bstSearch tree value, която приема подредено
двоично дърво tree и стойност value и връща дали стойността се среща в дървото.

Примери:
    tree   = (Node 4 
                (Node 2 
                    (Node 1 Empty Empty)
                    (Node 3 Empty Empty))
                (Node 5 
                    Empty
                    (Node 6 Empty Empty)))

    bstSearch tree 1 -> True
    bstSearch tree 4 -> True
    bstSearch tree 7 -> False
-}
bstSearch :: (Ord a) => Tree a -> a -> Bool
bstSearch Empty _ = False
bstSearch (Node n l r) value
  | n == value = True
  | otherwise  = bstSearch l value || bstSearch r value

{-
Задача 6. Дефинирайте функцията bstInsert tree value, която приема подредено
двоично дърво tree и стойност value и добавя value в дървото (като го оставя
наредено).

Примери:
    tree   = (Node 4 
                (Node 2 
                    (Node 1 Empty Empty)
                    (Node 3 Empty Empty))
                (Node 5 
                    Empty
                    (Node 6 Empty Empty)))

    bstInsert tree 7 -> (Node 4 
                            (Node 2 
                                (Node 1 Empty Empty) 
                                (Node 3 Empty Empty)) 
                            (Node 5 
                                Empty 
                                (Node 6 
                                    Empty 
                                    (Node 7 Empty Empty))))
-}
bstInsert :: (Ord a) => Tree a -> a -> Tree a
bstInsert Empty value = Node value Empty Empty
bstInsert (Node n l r) value
  | n <= value = Node n l (bstInsert r value)
  | n > value  = Node n (bstInsert l value) r

{-
Задача 7. Дефинирайте функцията bstRemove tree value, която приема подредено
двоично дърво tree и стойност value и премахва value от дървото (като го оставя
наредено).

Примери:
    tree   = (Node 4 
                (Node 2 
                    (Node 1 Empty Empty)
                    (Node 3 Empty Empty))
                (Node 5 
                    Empty
                    (Node 6 Empty Empty)))

    bstRemove tree 4 -> (Node 2 
                            (Node 1 Empty Empty) 
                            (Node 3 
                                Empty 
                                (Node 5 
                                    Empty 
                                    (Node 6 Empty Empty))))
-}

-- But can we do it without getting the elems first and constructing a new tree?
bstRemove :: (Ord a) => Tree a -> a -> Tree a
bstRemove Empty _ = Empty
bstRemove (Node n l r) val = makeTree Empty (filter (/=val) (elems (Node n l r)))

makeTree :: (Ord a) => Tree a -> [a] -> Tree a
makeTree t [] = t
makeTree t (x:xs) = makeTree (bstInsert t x) xs

elems :: (Ord a) => Tree a -> [a]
elems Empty = []
elems (Node n l r) = [n] ++ elems l ++ elems r

-- main функция с примерни извиквания на функциите от задачите.
main :: IO()
main = let
    tree11 = (Node 3 
                (Node 1 
                    (Node 1 Empty Empty)
                    Empty)
                (Node 2 
                    (Node 4 Empty Empty)
                    (Node 7 Empty Empty)))
    tree21 = (Node 4 
                (Node 3 
                    (Node 1 Empty Empty)
                    (Node 2 Empty Empty))
                (Node 5 
                    Empty
                    (Node 6 Empty Empty)))
    tree22 = (Node 4 
                (Node 5 
                    (Node 6 Empty Empty)
                    Empty)
                (Node 3 
                    (Node 2 Empty Empty)
                    (Node 1 Empty Empty)))
    tree23 = (Node 4 
                (Node 5 
                    (Node 6 Empty Empty)
                    Empty)
                (Node 3 
                    (Node 1 Empty Empty)
                    (Node 2 Empty Empty)))
    tree31 = (Node 4 
                (Node 2 
                    (Node 1 Empty Empty)
                    (Node 3 Empty Empty))
                (Node 5 
                    Empty
                    (Node 6 Empty Empty)))
    tree32 = (Node 4 
                (Node 2 
                    (Node 1 Empty Empty)
                    (Node 3 Empty Empty))
                (Node 5 
                    (Node 6 Empty Empty)
                    Empty))
    -- Зад. 4-7.
    tree   = (Node 4 
                (Node 2 
                    (Node 1 Empty Empty)
                    (Node 3 Empty Empty))
                (Node 5 
                    Empty
                    (Node 6 Empty Empty)))
    in do
        -- Задача 1.
        print $ listSpecial tree11

        -- Задача 2.
        print $ areMirrorImages tree21 tree22
        print $ areMirrorImages tree21 tree23

        -- Задача 3.
        print $ isBST tree31
        print $ isBST tree32

        -- Задача 4.
        print $ bstToList tree

        -- Задача 5.
        print $ bstSearch tree 1
        print $ bstSearch tree 4
        print $ bstSearch tree 7
        
        -- Задача 6.
        print $ bstInsert tree 7

        -- Задача 7.
        print $ bstRemove tree 4        
