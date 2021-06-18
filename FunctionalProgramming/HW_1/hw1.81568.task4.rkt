#lang racket
(define first
  (lambda (al)
    (caar al)))

(define second
  (lambda (al)
    (cadar al)))

(define (max-f al)
  (define (iter al curr-f curr-max)
    (cond
      ((null? al) curr-f)
      ((> (second al) curr-max)
        (iter (cdr al)
          (first al) (second al)))
      (else
        (iter (cdr al) curr-f curr-max))))
  (iter (cdr al) (first al)
      (second al))) 
     
(define (maximize lf)
  (lambda (x)
    ((max-f
      (map
        (lambda (f)
          (list f (abs (f x))))
        lf)) x)))
