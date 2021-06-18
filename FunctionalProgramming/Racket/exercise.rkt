#lang racket
(define (compose-n f g n)
  (lambda (x)
    (cond
      ((zero? n) x)
      ((= (modulo n 2) 0)
        (g ((compose-n f g (- n 1)) x)))
      (else
        (f ((compose-n f g (- n 1)) x))))))

(define (switchsum f g n)
  (λ (x)
    (apply +
      (map
        (λ (cnt)
          ((compose-n f g (+ cnt 1)) x))
        (range n)))))

((switchsum (λ(x) (+ x 1)) (λ(x) (* x 2)) 1) 2)
((switchsum (λ(x) (+ x 1)) (λ(x) (* x 2)) 2) 2)
((switchsum (λ(x) (+ x 1)) (λ(x) (* x 2)) 3) 2)
((switchsum (λ(x) (+ x 1)) (λ(x) (* x 2)) 4) 2)

(define (foldl1 f nv lst)
  (cond
    ((null? lst) nv)
    (else
      (f (car lst)
        (foldl1 f nv (cdr lst))))))

(define (atom? a) (and (not (pair? a)) (not (null? a))))

(define (deep-fold ll)
  (cond
    ((null? ll) ll)
    ((atom? ll) (list ll))
    (else (append
      (deep-fold (car ll))
      (deep-fold (cdr ll))))))

(deep-fold '(1 2 3 (4 (5 (6)))))