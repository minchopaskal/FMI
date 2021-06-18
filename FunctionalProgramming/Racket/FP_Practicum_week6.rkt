#lang racket
(define (insert-at pos x lst)
  (cond [(null? lst) (list x)]
        [(= pos 0) (cons x lst)]
        [else (cons (car lst) (insert-at (- pos 1)
                                         x
                                         (cdr lst)))]))

(define (in-all-positions x lst)
  (map (lambda (pos) (insert-at pos x lst))
       (range (+ 1 (length lst)))))

(define (permutations lst)
  (define (helper n res lst)
    (cond
      ((= n 1) res)
      (else (helper (- n 1) (apply append
                       (map (lambda (zst)
                             (in-all-positions (car lst) zst))
                     res))
                (cdr lst)))))
  (helper 3 '(()) lst))

(permutations '(1 2 3 4))