#lang racket

;; Helpers
(define (to-int l)
  (define (help l int pow)
    (cond
      ((null? l) int)
      (else (help (cdr l)
        (+ (* (car l) (expt 10 pow)) int)
          (+ 1 pow)))))
  (help (reverse l) 0 0))
;; end

;; Main functions
(define (get-remainders x n)
  (define (help x remainders)
    (cond
      ((zero? x) remainders)
      (else (help (quotient x n)
        (cons
          (modulo x n) remainders)))))
  (help x '()))

(define (convert-decimal x k)
  (define (help x converted pow)
    (cond
      ((zero? x) converted)
      (else
        (help (quotient x 10)
          (+ (* (modulo x 10)
            (expt k pow)) converted)
              (+ pow 1)))))
  (help x 0 0))

(define (convert x k n)
  (cond
    ((= n 10)
      (convert-decimal x k))
    (else
      (to-int
        (get-remainders
          (convert-decimal x k) n)))))
  
    