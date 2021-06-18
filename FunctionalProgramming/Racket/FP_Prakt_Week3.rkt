#lang racket

(define (sum-del n)
  (define (get-dels start sum)
    (cond
      ((= start n) sum)
      ((= (modulo n start) 0)
       (get-dels (+ start 1) (+ start sum)))
      (else
       (get-dels (+ start 1) sum))))
  (get-dels 1 0))

(define (perfect-number? n)
      (= (sum-del n) n))

(define (inc-digits? n)
  (define (inc-help n prev)
    (cond
      ((<= n 10) (< n prev))
      ((<= (modulo n 10) prev)
       (inc-help (quotient n 10) (modulo n 10)))
      (else #f)))
  (inc-help (quotient n 10) (modulo n 10)))

(define (automorphic? n)
  (define (help n-sq n)
    (cond
      ((< n 10) (= (modulo n-sq 10) n))
      ((= (modulo n-sq 10) (modulo n 10))
      (help (quotient n-sq 10) (quotient n 10)))
      (else #f)))
  (help (* n n) n))

(define (pow x n)
  (cond
    ((= n 0) 1)
    (else (* x (pow x (- n 1))))))

(define (sum x n)
    (cond
      ((= n 0) 1)
      (else (+ (pow x n) (sum x (- n 1))))))

(define (sum-fancy x n)
  (define (help acc n)
    (cond
      ((= n 0) 1)
      (else
       (+ acc (help (* x acc) (- n 1))))))
  (help x n))
