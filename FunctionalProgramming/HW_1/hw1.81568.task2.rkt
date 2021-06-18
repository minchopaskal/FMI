#lang racket

(define (to-int l)
  (define (help l num pow)
    (cond
      ((null? l) num)
      (else (help (cdr l)
        (+ (* (car l) (expt 10 pow)) num) (+ 1 pow)))))
  (help l 0 0))

(define (append-last x nums)
  (cond
    ((not (null? x))
      (cons (list x (car nums)) (cdr nums)))
    ((null? (car nums)) nums)
    (else
      (cons '() nums))))

(define (get-numbers str)
  (let ((zero #\0)
        (nine #\9)
        (zero-int 48))
  (define (help str nums)
    (cond
      ((null? str) nums)
      ((and (char>=? (car str) zero)
         (char<=? (car str) nine))
           (help (cdr str)
             (append-last
               (- (char->integer (car str))
                 zero-int) nums)))
      (else
        (help (cdr str)
          (append-last '() nums)))))
    (help str '(()))))
      
(define (sum-numbers str)
  (apply +
    (map (lambda (x)
      (to-int x))
    (get-numbers
      (string->list str)))))