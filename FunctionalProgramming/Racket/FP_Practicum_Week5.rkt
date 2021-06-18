#lang racket

;; Help functions

(define (my-filter p? xs)
  (if (null? xs)
      null
      (let* ([head (car xs)]
             [tail (cdr xs)]
             [filter-tail (my-filter p? tail)])
        (if (p? head)
            (cons head filter-tail)
            filter-tail))))

(define (sum xs)
  (apply + xs))

(define (divides? k n)
  (= (remainder n k) 0))

(define (prime? n)
  (and (> n 1)
    (null?
      (my-filter
        (lambda (k)
          (divides? k n))
        (range 2 (+ 1 (floor (sqrt n))))))))

(define (list-of n)
  (define (help m)
    (cond
      ((> m n) '())
      (else
       (cons m (help (add1 m))))))
  (help 2))

(define square
  (lambda (x)
    (* x x)))

;; Help functions for work with list of pairs
(define first
  (lambda (pxs)
    (car (car pxs))))

(define second
  (lambda (pxs)
    (cdr (car pxs))))

;; Ex. 0 - Useful operations over lists

(define (minimum xs) (apply min xs))
(define (maximum xs) (apply xs max))
(define (product xs) (apply xs *))
(define (concat xss) (apply xss append))

;; Зад 1. Да се дефинира функцията (count-minimum xs), която връща
;; броя на срещанията на най-малкия елемент на списъка xs в него.
(define (count-minimum xs)
  (define (help xs2)
    (cond
      ((null? xs2) 0)
      ((equal? (car xs2) (minimum xs))
       (+ 1 (help (cdr xs2))))
      (else
       (help (cdr xs2)))))
  (help xs))

;; Задача 2. Да се дефинира функцията (count-occurrences xs subs),
;; която по зададени списъци xs и subxs връща като резултат броя на
;; срещанията на списъка subs в списъка xs.
(define (get-subxs xs n)
  (cond
    ((= n 0) '())
    ((< (length xs) n) '())
    (else
     (cons (car xs)
       (get-subxs (cdr xs)
         (- n 1))))))

(define (count-occurrences xs subs)
  (cond
    ((null? xs) 0)
    ((< (length xs) (length subs)) 0)
    ((equal? (get-subxs xs
      (length subs)) subs)
        (+ 1 (count-occurrences (cdr xs) subs)))
    (else (count-occurrences (cdr xs) subs))))

;; Задача 3. Дефинирайте следните функции:
;; a). (my-identity x), функцията идентитет: връща каквото и дадете.
;; б). (my-compose f g), която връща композицията на функциите f и g.
;; в). (my-negate p?), която приема предикат p? и връща предиката (not p?).
;; г). (my-curry f x), която приема многоаргумента функция f и първи аргумент x
;;      и връща функцията получена от частичното прилагане на x върху f.
;; д). (repeatf f n), която връща n-кратната композиция на функцията f.

;; a).
(define (my-identity x) x)

;; b).
(define (my-compose f g)
  (lambda (x)
    (f (g x))))

;; c).
(define (my-negate p?)
  (not p?))

;; d). ???
(define (my-curry f x)
  (lambda (y)
    (f x y)))
  
(define add1 (my-curry + 1))

;; e).
(define (repeatf f n)
  (cond
    ((= n 1) f)
    (else
     (lambda (x)
       (f ((repeatf f (- n 1)) x))))))

;; Задача 4*. Решето на Ератостен:
;; Да се дефинира функцията (sieve-of-eratosthenes n), която приема
;; целочисления аргумент n и връща списък с всички прости числа
;; по-малки или равни на n, като за целта използвайте решетото на
;; Ератостен.

;; Used source https://en.wikipedia.org/wiki/Integer_factorization

(define (e-sieve n)
  (define (remove-from n sieve)
    (cond
      ((null? sieve) '())
      ((= (modulo (car sieve) n) 0)    
        (remove-from n (cdr sieve)))
      (else
        (cons (car sieve)
          (remove-from n (cdr sieve))))))
  (define (help curr curr-primes sieve)
    (let ([removed (remove-from curr sieve)])
      (cond
        ((null? removed)
          (append curr-primes
            (cons curr '())))
        (else
          (help (car removed)
            (append curr-primes
              (cons curr '())) removed)))))
  (let ([sieve (list-of n)])
    (cond
      ((< n 2) '())
      (else (help 2 '() sieve)))))

(define (e-sieve-faster n)
  (define (remove-from n sieve)
    (cond
      ((null? sieve) '())
      ((and (not (= n (car sieve)))
       (= (modulo (car sieve) n) 0))    
        (remove-from n (cdr sieve)))
      (else
        (cons (car sieve)
          (remove-from n (cdr sieve))))))
  (define (help curr sieve less-sieve)
    (let* ([removed (remove-from curr sieve)]
           [less (cdr (remove-from curr less-sieve))])
      (cond
        ((equal? sieve removed) sieve)
        (else
          (help (car less)
            removed less)))))
  (let ([sieve (list-of n)])
    (cond
      ((< n 2) '())
      (else (help 2 sieve sieve)))))
       
;; Tests
(display "Sieve of Erathosten tests:\n")
(equal? (e-sieve-faster 10) '(2 3 5 7))
(equal? (e-sieve-faster 100) '(2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 53 59 61 67 71 73 79 83 89 97))
(= (length (e-sieve-faster 1000)) 168)
#;(e-sieve-faster 1000000)
#;(time (e-sieve-faster 1000000))

; Задача 5*. Разлагане на прости делители:
; Да се дефинира функцията (prime-factors n), която приема
; целочисления аргумент n и връща списък от двойки (pairs)
; от тип (pi . ki), където pi e i-тия прост делител на n, a
; ki степента на pi във факторизацията на n.

;; Used source https://en.wikipedia.org/wiki/Trial_division
;; > (= trial-division how-we-searched-prime-factors-in-scool)
;; #t
(define (prime-factors n)
  (define (add-to-pxs p pxs)
    (cond
      ((null? pxs) (cons (cons p '1) '()))
      ((null? (car pxs)) (cons (cons p '1) '()))
      ((= p (first pxs))
        (cons
          (cons p (add1 (second pxs)))
          (cdr pxs)))
      (else
        (cons (car pxs)
          (add-to-pxs p (cdr pxs))))))
  
  (define (check-factor n p curr-primes curr-factors)
    (cond
      ((= (modulo n p) 0)
       (check-factor (quotient n p) p
         curr-primes
           (add-to-pxs p curr-factors)))
      (else (get-primes n curr-primes curr-factors))))
  
  (define (get-primes n primes curr-factors)
    (cond
      ((> (square (car primes)) n)
       (cond
         ((> n 1) (add-to-pxs n curr-factors))
         (else curr-factors)))
      (else
        (check-factor n
          (car primes) (cdr primes)
            curr-factors))))
  
  (get-primes n (e-sieve-faster (add1 (exact-round (sqrt n)))) '(()) ))
  
;; Tests
(display "Prime Factorization tests:\n")
(equal? (prime-factors 2) '((2 . 1)))
(equal? (prime-factors 3) '((3 . 1)))
(equal? (prime-factors 10) '((2 . 1) (5 . 1)))
(equal? (prime-factors 12) '((2 . 2) (3 . 1)))
(equal? (prime-factors 97) '((97 . 1)))
(equal? (prime-factors 360) '((2 . 3) (3 . 2) (5 . 1)))
(equal? (prime-factors 792) '((2 . 3) (3 . 2) (11 . 1)))
(equal? (prime-factors 1024) '((2 . 10)))

      
      
    



















  