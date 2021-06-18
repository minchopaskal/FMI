#lang racket
;;; Representing graph with adjacency lists
(define g '((a b e)
            (b c)
            (c a d)
            (d a e)
            (e)))

;;; Returns a list of all adjacent nodes to
;;; the node 'a' in the graph 'g'
(define (children a g)
  (cdr (assq a g)))

;;; Given a list of nodes checks wether it's cycled
(define (cycled? l)
  (cond
    ((null? l) #f)
    ((memq (car l)
      (cdr l)) #t)
    (else
      (cycled? (cdr l)))))
  
;;; Returns a list of possible paths
;;; given a list of nodes
(define (get-paths path g)
    (map
      (λ (x)
        (cons x path))
      (children (car path) g)))

;;; Returns a list of all possible
;;; next paths
;;; given a list of paths
(define (get-all-paths ll g)
  (apply append
    (map
      (λ (path)
        (get-paths path g))
      ll)))

;;; Given a list of paths checks
;;; if node 'd' is reached
(define (reached? d lp)
  (let ((valid-paths
    (filter
      (λ (path)
        (equal? d (car path)))
      lp)))
    (and (not
      (null? valid-paths))
        (car valid-paths))))
   
;;; Checks if there is a path
;;; from node 'a' to node 'b' in graph 'g'
;;; Returns: #f if there isn't.
;;;          arbitrary path, otherwise
(define (has-path a b g)
  (define (iter paths)
    (cond
      ((null? paths) #f)
      (else (or (reached? b paths)
        (iter (get-all-paths paths g))))))
  (iter (list (list a))))

(has-path 'a 'd g)
(has-path 'c 'e g)
(has-path 'e 'a g)

