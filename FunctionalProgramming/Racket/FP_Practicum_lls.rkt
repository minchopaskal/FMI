#lang racket

;; Да се дефинира предикат (triangular? mat), който получава квадратна числова матрица,
;; представена като списък от списъци, и проверява дали тя е горно триъгълна, т.е. дали всичките
;; елементи под главния й диагонал са нули.
(define get-row car)
(define (get-col mat) (map car mat))

(define (triangular? mat)
  (cond
    ((null? mat) #t)
    ((null? (filter (lambda (x) (not (zero? x))) (cdr (get-col mat))))
     (triangular? (map cdr (cdr mat))))
    (else #f)))
     
(triangular? '((1 2 3) (0 5 6) (0 0 9)))
(triangular? '((0 2 3) (0 0 6) (1 0 0)))

(define (compose f g)
  (λ (x)
    (f (g x))))

(define id
  (λ (x) x))

(define (pair-compose fs)
  (cond
    ((null? fs) (lambda (x) 0))
    ((null? (cdr fs)) (car fs))
    (else
      (λ (x)
        (+ ((compose (car fs) (cadr fs)) x)
           ((pair-compose (cddr fs)) x) )))))

(define add1
  (λ(x) (+ 1 x)))
(define add2
  (λ (x) (+ 2 x)))

(define g (pair-compose (list add1 add2)))
(define g2 (pair-compose (list add1 add2 add1)))