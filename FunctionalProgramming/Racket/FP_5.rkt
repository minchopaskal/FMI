#lang racket

(define add1
  (lambda (x)
    (+ 1 x)))

;; Зад 1. range(a b)
(define (range a b)
  (cond
    ((= a b) (cons a '()))
    (else (cons a (range (add1 a) b)))))

;; Зад 2. sum-odd(lst)
(define sum-odd
  (lambda (lst)
    (cond
      ((null? lst) 0)
      ((= (modulo (car lst) 2) 1)
       (+ (car lst) (sum-odd (cdr lst))))
      (else (sum-odd (cdr lst))))))

;; Зад. 3 filter-even(lst)
(define filter-even
  (lambda (lst)
    (cond
      ((null? lst) '())
      ((= (modulo (car lst) 2) 0)
       (cons (car lst) (filter-even (cdr lst))))
      (else
       (filter-even (cdr lst))))))

;; Зад 4. intersect (set1 set2)
(define (intersect set1 set2)
  (define is-in-set
    (lambda (a set)
      (cond
        ((null? set) #f)
        ((equal? a (car set)) #t)
        (else (is-in-set a (cdr set))))))
    (cond
      ((null? set1) '())
      ((null? set2) '())
      ((is-in-set (car set1) set2)
       (cons (car set1) (intersect (cdr set1) set2)))
      (else (intersect
        (cdr set1) set2))))