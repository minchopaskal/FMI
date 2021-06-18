import Data.List

-- Alias for more clarity
type Point = (Int, Int)

gameOfLife :: [Point] -> [Point]
gameOfLife board = (sort . nub . help) board
  where aliveNeighbours   = length . filter isAlive . neighbours
        neighbours (x,y)  = filter (/=(x,y))
                            [(x+k,y+l) | k <- [-1..1], l <- [-1..1]]
        isAlive p         = elem p board
        newAlive          = filter (\p -> aliveNeighbours p == 3 ) . neighbours
        help [] = []
        help (p:rest)
          | aliveNeighbrs == 2 ||
            aliveNeighbrs == 3 = p : newRest
          | otherwise              =     newRest
          where newRest = newAlive p ++ help rest
                aliveNeighbrs = aliveNeighbours p
                                  
