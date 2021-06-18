main :: IO()
main = do
  print(count_digits 0)

{-
  Зад. 1. Да се дефинира процедура count_digits :: Int -> Int, която генерира линейно
  рекурсивен процес и намира броя на цифрите на дадено естествено число n.
-}
count_digits :: Int -> Int
count_digits 0 = -1
count_digits n = if n < 10 then 1 else 1 + count_digits(n `div` 10)

{-
  Зад. 2. Да се дефинира процедура sum_digits :: Int -> Int, която генерира линейно
  рекурсивен процес и намира сумата от цифрите на дадено естествено число n.
-}
sum_digits :: Int -> Int
sum_digits n = if  n < 10
               then n
               else n `mod` 10 + sum_digits(n `div` 10)
                   
{-
  Зад. 3. Да се дефинира процедура pow :: Double -> Int -> Double, която генерира линейно рекурсивен
  процес и намира x на степен n, където x е реално, а n - естествено число.
-}
pow :: Double -> Int -> Double
pow x n = if n == 0 then 1 else x * pow x (n - 1)

{-
  Зад. 4. Да се дефинира процедура sum_digits_iter :: Int -> Int, която генерира линейно
  итеративен процес и намира сумата от цифрите на дадено естествено число n.
-}
sum_digits_iter :: Int -> Int
sum_digits_iter n = helper n 0
  where
    helper 0 sum = sum
    helper n sum = helper (n `div` 10) (sum + (n `mod` 10))
                                 
{-
  Зад. 5. Да се дефинира процедура rev :: Int -> Int, която генерира линейно итеративен
  процес и по дадено естествено число n намира числото, записано със същите цифри,
  но в обратен ред.
-}
rev :: Int -> Int
rev n = iter n (count_digits n - 1)
  where
    iter 0 _ = 0
    iter n p = ((n `mod` 10) * (10^p)) + iter (n `div` 10) (p - 1)

{-
  Зад. 6. Да се дефинира предикат prime :: Int -> Bool, който проверява дали дадено естествено
  число n е просто.
  Забележка: Числото 1 не е нито просто, нито съставно.
-}
prime :: Int -> Bool
prime n
  | n < 2 = False
  | otherwise = iter n (round (sqrt (fromIntegral n)))
  where yiter n k
          | k < 2 = True
          | n `mod` k == 0 = False
          | otherwise = iter n (k - 1)
