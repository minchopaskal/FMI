#lang racket

;; Помощни функция
(define (last-digit n) (remainder n 10))
(define (front-digits n) (quotient n 10))

(define atom?
  (lambda (x)
    (and (not (pair? x)) (not (null? x)))))

(define add1
  (lambda (x)
    (+ x 1)))

(define sub1
  (lambda (x)
    (- x 1)))

(define member?
  (lambda (x xs)
    (cond
      ((null? xs) #f)
      ((eq? x (car xs)) #t)
      (else (member? x (cdr xs))))))

; Задача 1. Да се дефинира функцията (my-reverse xs),
; връщата списък с елементите на xs в обратен ред.
(define (my-reverse xs)
  (cond
    ((null? (cdr xs)) (cons (car xs) '()))
    (else
     (append
       (my-reverse (cdr xs))
         (cons (car xs) '())))))

; Задача 2. Да се дефинира функцията (my-append xs ys),
; която конкатенира списъците xs и ys.
(define (my-append xs ys)
  (cond
    ((null? xs) ys)
    (else
     (cons (car xs)
       (my-append (cdr xs) ys)))))

; Задача 3. Да се дефинира функцията (my-remove-duplicates xs),
; която премахва повтарящите се елементи от списъка xs.
(define (my-remove-duplicates xs)
  (define (help xs)
    (cond
      ((null? xs) '())
      ((member? (car xs)
         (cdr xs))
       (my-remove-duplicates (cdr xs)))
      (else
       (cons (car xs) (help (cdr xs))))))
  (help (reverse xs)))

; Задача 4. Да се дефинира функцията (digits n),
; връщата списък с цифрите на числото n.
(define digits
  (lambda (n)
    (cond
      ((zero? (front-digits n)) (cons n '()))
      (else
       (append
         (digits (front-digits n))
         (cons (last-digit n) '()))))))

; Задача 5*. Бикове и крави:
; Да се дефинира функцията (bulls-and-cows secret guess),
; която приема `тайното` число secret и `предположението` guess
; и връща двойка (pair), чийто първи елемент е броя на `биковете`
; в guess (т.е. на цифрите в guess, който се срещат на същата позиция
; в secret), a втори - броя на `кравите` (цифри на guess, които
; се срещат в secret, но са на различна позиция).

(define is-bull?
  (lambda (secret guess)
    (= (last-digit secret) (last-digit guess))))

(define is-in-number?
  (lambda (digit number)
    (cond
      ((zero? number) #f)
      ((eq? digit (last-digit number))
       #t)
      (else
        (is-in-number? digit
          (front-digits number))))))

(define bulls-count
  (lambda (secret guess)
    (cond
      ((zero? secret) 0)
      ((is-bull? secret guess)
       (add1 (bulls-count
         (front-digits secret)
           (front-digits guess))))
      (else
        (bulls-count
         (front-digits secret)
           (front-digits guess))))))

(define (cows-count secret guess)
  (define (help secret curr-guess)
    (cond
      ((zero? secret) 0)
      ((is-in-number? (last-digit secret) guess)
       (cond
         ((not (is-bull? secret curr-guess))
          (add1
            (help
              (front-digits secret)
                (front-digits curr-guess))))
         (else 
           (help (front-digits secret)
             (front-digits curr-guess)))))
      (else
        (help
          (front-digits secret)
            (front-digits curr-guess)))))
  (help secret guess))
       

(define bulls-and-cows
  (lambda (secret guess)
    (cons (bulls-count secret guess)
      (cows-count secret guess))))

; Задача 6*. Да се дефинира функцията (pick n xs), която връща списък с
; всички възможни избора на n елемента от списъка xs.

(define cons-to-lists
  (lambda (a as)
    (cond
      ((null? as) '())
      (else
       (cons (cons a (car as))
         (cons-to-lists a (cdr as)))))))

(define listify
  (lambda (xs)
    (cond
      ((null? xs) '())
      (else (cons
        (cons (car xs) '())
          (listify (cdr xs)))))))
        
(define (pick n xs)
    (cond
      ((null? xs) '())
      ((zero? n) '(()))
      (else
        (append 
          (cons-to-lists (car xs)
            (pick (sub1 n) (cdr xs)))
              (pick n (cdr xs))))))

(pick 0 '(1 2 3 4 5))
(pick 1 '(1 2 3 4 5))
(pick 2 '(1 2 3 4 5))
(pick 3 '(1 2 3 4 5))
(pick 4 '(1 2 3 4 5))
(pick 5 '(1 2 3 4 5))
(pick 6 '(1 2 3 4 5))
