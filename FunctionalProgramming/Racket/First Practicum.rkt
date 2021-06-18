#lang racket
(define (fact n)
  (if (= n 0)
      1
      (* n (fact (- n 1)))))

(define (sq x)
  ( * x x)
)

(define (avgSq x y)
  (/ (+ (sq x) (sq y)) 2 )
)

(define (fib-iter n)
  (define (helper n-2 n-1 counter)
    (if (= counter n)
        (+ n-1 n-2)
        (helper n-1 (+ n-1 n-2) (+ counter 1))))
  (if (< n 2)
      1
      (helper 1 1 2)))

(fib-iter 5)
(fib-iter 100)
