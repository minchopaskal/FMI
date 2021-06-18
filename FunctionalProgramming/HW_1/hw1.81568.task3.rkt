#lang racket
(define first caar)
(define second cadar)

(define (add-mapl x al)
  (cond
    ((or (null? al) (not (eq? (first al) x)))
      (cons (list x 1)
        al))
    (else
      (cons (list (first al)
        (+ 1 (second al)))
          (cdr al)))))

(define (encode l)
  (define (help l mapl)
    (cond
      ((null? l) mapl)
      (else
        (help (cdr l)
          (add-mapl (car l) mapl)))))
  (reverse (help l '())))

