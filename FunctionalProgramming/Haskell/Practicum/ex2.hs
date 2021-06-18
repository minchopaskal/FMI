{- Задачи -}

accumulate op nv a b next term
  | a > b     = nv
  | otherwise = term a `op` accumulate op nv (next a) b next term


{-
Задача 1. Да се дефинира функция digits n, която връща списък с цифрите на 
цялото число n >= 0.

Примери: 
    digits 1234 = [1, 2, 3, 4]
    digits 1750 = [1, 7, 5, 0]
-}

digits' :: (Integral t) => t -> [t]
digits' 0 = []
digits' n = digits' (n `div` 10) ++ [ n `mod` 10]

{-
Задача 2. Да се дефинира функция times n x, която получава цяло положително число 
n и стойност от произволен тип x и връща списък съдържащ x, повторено n пъти.

Примери:
    2 `times` 7 = [7, 7]
    7 `times` 2 = [2, 2, 2, 2, 2, 2, 2]
-}
times :: (Integral t) => t -> a -> [a]
n `times` x
  | n == 0    = []
  | otherwise = times (n - 1) x ++ [x]


{-
Задача 3.
a). Напишете функцията collatz n, дефинирана по-следния начин:
    - ако n e равно на 1, функцията връща 1.
    - ако n е четно, функцията връща n делено на 2.
    - ако n е нечетно, функията връща 3n + 1.
    Параметърът n e цяло число.

Примери:
    collatz 1 = 1
    collatz 4 = 2
    collatz 3 = 10

b). Напишете функцията areCollatzSequence, която приема списък от цели числа и връща 
дали всеки от елементите (с изключение на първия) се получава посредством извикването
на функцията collatz върху предишния елемент от списъка.

Примери:
    areCollatzSequence [3, 10, 5, 16, 8, 4, 2, 1] = True
    areCollatzSequence [9, 28, 14, 7, 22, 11, 34, 17, 52, 26, 13] = True
    areCollatzSequence [1, 3, 10] = False
-}
collatz :: (Integral t) => t -> t
collatz n
  | n == 1         = 1
  | n `mod` 2 == 0 = n `div` 2
  | otherwise      = 3 * n + 1

areCollatzSequence :: (Integral t) =>  [t] -> Bool
areCollatzSequence [] = True
areCollatzSequence (_:[]) = True
areCollatzSequence (x:xs)
  | collatz x == head xs = areCollatzSequence (tail xs)
  | otherwise              = False

{-
Задача 4. Mergesort: mergesort e един от най-ефикасните алгоритми за сортиране, особено що се отнася до 
функционални езици. Функцията sort от модула Data.List, която е стандартния начин за сортиране на списъци
в Haskell, използва - макар и малко по-оптимизиран от този, който ще имплементирате - вариант на този алгоритъм.

а). Нaпишете функцията merge xs ys, която приема два списъка подредени в нарастващ ред и ги 
обединява в един списък, чийто елементи също са подредени в нарастващ ред.

    Пример: merge [1, 3, 7] [2, 4, 6] = [1, 2, 3, 4, 6, 7] 

б). Използвайте функцията от предишната подточка и идеята, че мога да сортирам списък като го
разделя на две половини, сортирам всяка от тях поотделно и после ги обединя - което е пример за
т. нар. подход на разделяй и владей (divide and conquer) - за да напишете функция mergesort xs,
която приема списък xs и връща списък с елементите на xs сортирани в нарастващ ред.

    Пример: mergesort [2, 1, 3, 7, -16, 5] = [-16, 1, 2, 3, 5, 7]
-}
merge :: Ord a => [a] -> [a] -> [a]
merge xs ys
  | null xs               = ys
  | null ys               = xs
  | (head xs) < (head ys) = head xs : merge (tail xs) ys
  | otherwise             = head ys : merge xs (tail ys)

mergesort :: Ord a => [a] -> [a]
mergesort [] = []
mergesort (x:[]) = [x]
mergesort xs = merge (mergesort (fst spl)) (mergesort (snd spl))
               where spl = splitAt ((length xs) `div` 2) xs


{-
Задача 5. Трансформация на Бъроус-Уилър (Burrows-Wheeler transform)

Трансформацията на Бъроус-Уилър, е трансформация върху символни низове, която има
интересното свойство да групира еднаквите символи в низа близо един до друг. 
Поради тази причина, тя се използвана като предварителна стъпка в алгоритмите за 
компресия на данни, както и в биоинформатиката.

Алгоритъмът за намирането и е следния:
1. Генерираме всички ротации на входния низ, което ни дава списък от низове.
2. Сортираме списъка лексикографски.
3. Взимаме последния символ от всеки от редовете.

Пример:
    "BANANA" -- rotations --> [ "BANANA", -- sort --> [ "ABANAN", -- last symbol --> "NNBAAA"
                                "ANANAB",               "ANABAN",
                                "NANABA",               "ANANAB",
                                "ANABAN",               "BANANA",
                                "NABANA",               "NABANA",
                                "ABANAN" ]              "NANABA" ]

a). Напишете функцията rotate n str, която ротира низа str с n позиции наляво.

    Пример: rotate 1 "abc" = "bca"

б). Напишете функцията rotations str, която генерира всички ротации на str.

    Пример: rotations "abc" = ["abc", "bca", "cab"]

в). Напишете функцията bwt str, която приема низа str, и връща неговата трансформация
    на Бъроус-Уилър.

    Пример: bwt "BANANA" = "NNBAAA"
-}
rotate :: Int -> [a] -> [a]
rotate n xs
  | n == 0    = xs
  | otherwise = rotate (n - 1) (tail xs ++ [head xs])

rotations :: [a] -> [[a]]
rotations xs = iter (length xs) []
  where iter n curr
          | n == 0    = curr
          | otherwise = iter (n - 1) (rotate n xs : curr)

bwt :: Ord a => [a] -> [a]
bwt xs = [last x | x <- (mergesort (rotations xs))]


-- main функция с примерни извиквания на функциите от задачите.
main :: IO()
main = do 

    -- Задача 1.
    print $ digits' 1234
    print $ digits' 1750    

    -- Задача 2.
    print $ 2 `times` 7
    print $ 7 `times` 2

    -- Задача 3.
    print $ collatz 1
    print $ collatz 4
    print $ collatz 3
    print $ areCollatzSequence [3, 10, 5, 16, 8, 4, 2, 1]
    print $ areCollatzSequence [9, 28, 14, 7, 22, 11, 34, 17, 52, 26, 13]
    print $ areCollatzSequence [1, 3, 10]

    -- Задача 4.
    print $ merge [1, 3, 7] [2, 4, 6]
    print $ mergesort [2, 1, 3, 7, -16, 5]

    -- Задача 5.
    print $ rotate 1 "abc"
    print $ rotations "abc"
    print $ bwt "BANANA"
