#lang racket
(define (atom? x)
  (and
    (not (null? x))
      (not (pair? x))))
    

(define (flatten l)
  (cond
    ((null? l) l)
    ((atom? l) (list l))
    (else
      (append
        (flatten (car l))
          (flatten (cdr l))))))

(flatten '(1 2 (3 4 (5 6 (7) 8 9) 4 (5 6 7)) (7) () (8 9)))