#lang racket

;; Help functions
(define (sum xs) (apply + xs))
(define (product xs) (apply * xs))
(define add1 (lambda (x) (+ 1 x)))
(define sub1 (lambda (x) (- x 1)))
 
(define range
  (lambda (a b)
    (cond
      ((= a b) (cons a '()))
      (else
        (cons a (range (add1 a) b))))))

(define member?
  (lambda (x xs)
    (cond
      ((null? xs) #f)
      ((eq? x (car xs)) #t)
      (else
        (member? x
          (cdr xs))))))

(define rember*
  (lambda (x xs)
    (cond
      ((null? xs) '())
      ((= x (car xs))
        (rember* x (cdr xs)))
      (else
        (cons (car xs)
          (rember* x
            (cdr xs)))))))


; Задача 1. Да се дефинира функцията (sum-of-odd-squares xs),
; която връща сбора на квадратите на нечетните числа
; в списъка xs.
(define (sum-of-odd-squares xs)
  (sum
    (map
      (lambda (x)
        (cond
          ((= (modulo x 2) 1) (* x x))
          (else 0)))
      xs)))


; Задача 2. Да се дефинира функцията (num-bigger-elements xs),
; която за даден списък от числа xs връща като резултат
; списък с двойки (xi ni), където xi е i-тият елемент на xs,
; а ni е броят на елементите на xs, по-големи от xi.
(define (num-bigger-elements xs)
  (define (num-of-bigger x xs)
    (cond
      ((null? xs) 0)
      ((> (car xs) x)
        (+ 1 (num-of-bigger x
          (cdr xs))))
      (else
        (num-of-bigger x
          (cdr xs)))))
  (map
    (lambda (x)
      (cons x
        (num-of-bigger x xs)))
    xs))


; Задача 3. Да се дефинира функцията (sum-unique xss),
; която по списък от списъци от цели числа xss намира
; сумата на тези от числата, които са уникални
; в рамките на списъка, в който се срещат.
(define (sum-unique xss)
  (define unique
    (lambda (xs)
      (cond
        ((null? xs) '())
        ((member? (car xs) (cdr xs))
         (unique (rember* (car xs) xs)))
        (else
         (cons (car xs) (unique (cdr xs)))))))
  (sum
    (map
      (lambda (xs)
        (sum
          (unique xs)))
      xss)))


; Задача 4. Да се дефинира функцията (repeat n x), която приема
; целочислен аргумент n и произволен обект x и връща списък,
; състоящ се от x повторено n пъти.
#;(define (repeat-with-map n x) ;; <-- slow
  (map (lambda (y) x)
    (range 1 n))) 

(define (repeat n x) ;; <-- legit
  (cond
    ((eq? n 0) '())
    (else
      (cons x
        (repeat
          (sub1 n) x)))))


; Задача 5. Да се дефинира функцията (Id n), която
; приема целочислен аргумент n и връща единичната матрица
; с размер n (представена като списък от n линии, всеки -
; списък от n елемента).
(define (Id n)
  (map
    (lambda (x index)
      (map
        (lambda (indexs)
          (cond
            ((= indexs index) 1)
            (else 0)))
        (range 1 n)))
    (repeat n 0)
    (range 1 n)))


; Задача 6*. Да се дефинира функцията (transpose xss), която
; приема матрица xss (представена като списък от списъци) и
; връща транспонираната на xss матрица.
(define (transpose xss)
  (apply map list xss))

;; Tests
(displayln "Transpose tests:")
(equal? (transpose '((1 2 3 4 5 6))) '((1) (2) (3) (4) (5) (6)))
(equal? (transpose '((1 2) (3 4) (5 6))) '((1 3 5) (2 4 6)))
(equal? (transpose '((1 2 3) (4 5 6) (7 8 9))) '((1 4 7) (2 5 8) (3 6 9)))


; Задача 7**. Да се дефинира (на един ред :) функцията (matrix-multiply xss yss),
; която приема матриците xss и yss (представени като списъци от списъци)
; и връща тяхното матрично произведение.
(define (matrix-multiply xss yss) (map (λ (x-row) (map (λ (y-row) (apply + (map * x-row y-row))) (transpose yss))) xss)) ;; just too perverse

(matrix-multiply '((1 2 3) (4 5 6) (7 8 9)) '((1 0 0) (0 1 0) (0 0 1)))
(matrix-multiply '((1 2) (3 4)) '((5 6) (7 8)))