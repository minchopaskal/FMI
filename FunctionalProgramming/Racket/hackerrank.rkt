#lang racket

(define (read-list)
  (let ((n (read)))
    (if (eof-object? n)
        (list)
        (cons n (read-list)))))

(define (print-list l)
  (cond
    ((null? l) (void))
    (else (printf "~s\n" (car l))
          (print-list (cdr l)))))

(define (make-list n)
  (cond
    ((zero? n) '())
    (else (cons n (make-list (- n 1))))))
         