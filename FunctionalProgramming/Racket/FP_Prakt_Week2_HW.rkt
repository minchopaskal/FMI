#lang racket
(define (gcd a b)
  (cond
    ((= b 0) a)
    ((= a 0) b)
    ((< a b) (gcd a (modulo b a)))
    (else (gcd b (modulo a b)))))