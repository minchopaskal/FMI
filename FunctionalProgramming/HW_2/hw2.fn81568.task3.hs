data Tree a = Empty | Node a (Tree a) (Tree a) deriving (Read, Show)

left :: Tree a -> (Tree a)
left (Node _ l _) = l

right :: Tree a -> (Tree a)
right (Node _ _ r) = r

height :: Tree a -> Int
height Empty = 0
height tree  = 1 + max (height $ left tree) (height $ right tree)

isBalanced :: Tree a -> Int -> Bool
isBalanced Empty _ = True
isBalanced tree k
  = isBalanced leftTree k &&
    isBalanced rightTree k &&
    abs ((height leftTree) - (height rightTree)) <= k
    where leftTree  = left tree
          rightTree = right tree

