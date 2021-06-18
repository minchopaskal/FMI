#lang racket

;; Chapter 1. Toys
(define atom?
  (lambda (x)
    (and (not (pair? x)) (not (null? x)))))

(define lat?
  (lambda (x)
    (cond
      ((null? x) #t)
      ((atom? (car x))(lat? (cdr x)))
      (else #f))))

;; Chapter 2. Do It, Do It Again, and Again, and Again...

(define member?
  (lambda (a lat)
    (cond
      ((null? lat) #f)
      (else (or (eq? a (car lat))
             (member? a (cdr lat)))))))

;; Chapter 3. Cons the magnificent

(define rember
  (lambda (s l)
    (cond
      ((null? l) '())
      ((equal? s (car l)) (cdr l))
      (else (cons (car l)
               (rember s (cdr l)))))))

(define firsts
  (lambda (lat)
    (cond
      ((null? lat) '())
      (else (cons (car (car lat))
                  (firsts (cdr lat)))))))

#;(define insertR
  (lambda (new old lat)
    (cond
      ((null? lat) '())
      ((equal? (car lat) old)
       (cons (car lat)
             (cons new (cdr lat))))
      (else (cons (car lat)
                  (insertR new old (cdr lat)))))))

#;(define insertL
  (lambda (new old lat)
    (cond
      ((null? lat) '())
      ((equal? (car lat) old)
       (cons new lat))
      (else (cons (car lat)
                  (insertL new old (cdr lat)))))))

#;(define subst
  (lambda (new old lat)
    (cond
      ((null? lat) '())
      ((eq? (car lat) old) (cons new (cdr lat)))
      (else (cons (car lat)
                  (subst new old (cdr lat)))))))

(define subst2
  (lambda (new old1 old2 lat)
    (cond
      ((null? lat) '())
      ((or (eq? (car lat) old1)
           (eq? (car lat) old2))
       (cons new (cdr lat)))
      (else (cons (car lat)
                  (subst2 new old1 old2 (cdr lat)))))))

(define multirember
  (lambda (a lat)
    (cond
      ((null? lat) '())
      ((eq? a (car lat)) (multirember a (cdr lat)))
      (else (cons (car lat) (multirember a (cdr lat)))))))

(define multiInsertR
  (lambda (new old lat)
    (cond
      ((null? lat) '())
      ((eq? (car lat) old)
            (cons old (cons new
                            (multiInsertR new old (cdr lat)))))
      (else (cons (car lat) (multiInsertR new old (cdr lat)))))))

(define multiInsertL
  (lambda (new old lat)
    (cond
      ((null? lat) '())
      (else
       (cond
         ((eq? (car lat) old)
          (cons new
            (cons old
              (multiInsertL new old
                (cdr lat)))))
         (else
          (cons (car lat)
            (multiInsertL new old
              (cdr lat)))))))))

(define multisubst
  (lambda (new old lat)
    (cond
      ((null? lat) '())
      (else
       (cond
         ((eq? (car lat) old)
          (cons new
            (multisubst new old
              (cdr lat))))
         (else (cons (car lat)
                 (multisubst new old
                   (cdr lat)))))))))

;; Chapter 4. Numbers Games

(define add1
  (lambda (x)
    (+ x 1)))

(define sub1
  (lambda (x)
    (- x 1)))

(define plus
  (lambda (a b)
    (cond
      ((zero? b) a)
      (else (add1 (plus a (sub1 b)))))))

(define addtup
  (lambda (tup)
    (cond
      ((null? tup) 0)
      (else (plus (car tup) (addtup (cdr tup)))))))

(define mul
  (lambda (m n)
    (cond
      ((zero? n) 0)
      (else (plus m (mul m (sub1 n)))))))

(define tup+
  (lambda (tup1 tup2)
    (cond
      ((null? tup1) tup2)
      ((null? tup2) tup1)
      (else (cons (plus (car tup1) (car tup2))
        (tup+
          (cdr tup1) (cdr tup2)))))))

(define bigger
  (lambda (m n)
    (cond
      ((zero? m) #f)
      ((zero? n) #t)
      (else (bigger (sub1 m) (sub1 n))))))

(define smaller
  (lambda (m n)
    (cond
      ((zero? n) #f)
      ((zero? m) #t)
      (else (smaller (sub1 m) (sub1 n))))))

(define equal
  (lambda (m n)
    (cond
      ((smaller m n) #f)
      ((bigger m n) #f)
      (else #t))))

(define pow
  (lambda (n p)
    (cond
      ((equal n 1) 1)
      ((zero? p) 1)
      (else (* n (pow n (sub1 p)))))))

(define div
  (lambda (m n)
    (cond
      ((smaller m n) 0)
      (else (add1 (div (- m n) n))))))

(define length
  (lambda (lat)
    (cond
      ((null? lat) 0)
      (else (add1 (length (cdr lat)))))))

(define pick
  (lambda (n lat)
    (cond
      ((null? lat) (quote ()))
      ((zero? (sub1 n)) (car lat))
      (else (pick (sub1 n) (cdr lat))))))

(define rempick
  (lambda (n lat)
    (cond
      ((null? lat) '())
      ((zero? (sub1 n)) (cdr lat))
      (else (cons (car lat)
              (rempick (sub1 n) (cdr lat)))))))

(define no-nums
  (lambda (lat)
    (cond
      ((null? lat) '())
      ((number? (car lat)) (no-nums (cdr lat)))
      (else (cons (car lat) (no-nums (cdr lat)))))))

(define all-nums
  (lambda (lat)
    (cond
      ((null? lat) '())
      (else
       (cond
         ((number? (car lat))
           (cons (car lat)
             (all-nums (cdr lat))))
         (else (all-nums (cdr lat))))))))

(define eqan?
  (lambda (a b)
    (cond
      ((and (number? a) (number? b))
       (= a b))
      ((or (number? a) (number? b))
       #f)
      (else (eq? a b)))))

(define occur
  (lambda (a lat)
    (cond
      ((null? lat) 0)
      ((eqan? a (car lat)) (add1 (occur a (cdr lat))))
      (else (occur a (cdr lat))))))

(define one?
  (lambda (n)
    (eqan? n 1)))

(define rempick-new
  (lambda (n lat)
    (cond
      ((one? n) (cdr lat))
      (else (cons (car lat)
              (rempick-new (sub1 n)
                 (cdr lat)))))))

;; Chapter 5. *Oh My Gawd*: It's Full of Stars

(define rember*
  (lambda (a l)
    (cond
      ((null? l) '())
      ((atom? (car l))
       (cond
         ((eq? (car l) a)
          (rember* a (cdr l)))
         (else (cons (car l)
            (rember* a (cdr l))))))
      (else (cons (rember* a (car l))
       (rember* a (cdr l)))))))

(define insertR*
  (lambda (new old l)
    (cond
      ((null? l) '())
      ((atom? (car l))
       (cond
         ((eq? (car l) old)
          (cons old ( cons new
            (insertR* new old (cdr l)))))
         (else
          (cons (car l)
            (insertR* new old (cdr l))))))
      (else
       (cons (insertR* new old (car l))
         (insertR* new old (cdr l)))))))

(define occur*
  (lambda (a l)
    (cond
      ((null? l) 0)
      ((atom? (car l))
       (cond
         ((eq? (car l) a)
          (add1 (occur* a (cdr l))))
         (else
          (occur* a (cdr l)))))
      (else
       (plus (occur* a (car l))
        (occur* a (cdr l)))))))

(define subst*
  (lambda (new old l)
    (cond
      ((null? l) '())
      ((atom? (car l))
       (cond
         ((eq? (car l) old)
          (cons new
            (subst* new old (cdr l))))
         (else (cons (car l)
                 (subst* new old
                    (cdr l))))))
      (else
       (cons (subst* new old (car l))
          (subst* new old (cdr l)))))))

(define insertL*
  (lambda (new old l)
    (cond
      ((null? l) '())
      ((atom? (car l))
        (cond
          ((eq? (car l) old)
            (cons new
              (cons old
                (insertL* new old
                  (cdr l)))))
          (else (cons (car l)
                  (insertL* new old
                    (cdr l))))))
      (else (cons (insertL* new old
                    (car l))
              (insertL* new old
                (cdr l)))))))

(define member*
  (lambda (a l)
    (cond
      ((null? l) #f)
      ((atom? (car l))
       (cond
         ((eq? a (car l)) #t)
         (else (member* a
           (cdr l)))))
      (else (or (member* a (car l))
              (member* a (cdr l)))))))

(define leftmost
  (lambda (l)
    (cond
      ((atom? (car l)) (car l))
      (else (leftmost (car l))))))


#;(define eqlist?
  (lambda (l1 l2)
    (cond
      ((and (null? l1) (null? l2)) #t)
      ((or (null? l1) (null? l2)) #f)
      ((and (atom? (car l1))
         (atom? (car l2)))
       (and (eqan? (car l1) (car l2))
         (eqlist? (cdr l1) (cdr l2))))
      ((or (atom? (car l1))
         (atom? (car l2)))
       #f)
      (else
        (and (eqlist? (car l1) (car l2))
            (eqlist? (cdr l1) (cdr l2)))))))

(define equal?
  (lambda (s1 s2)
    (cond
      ((and (atom? s1) (atom? s2))
            (eqan? s1 s2))
      ((or (atom? s1) (atom? s2)) #f)
      (else (eqlist? s1 s2)))))

(define eqlist?
  (lambda (l1 l2)
    (cond
      ((and (null? l1) (null? l2)) #t)
      ((or (null? l1) (null? l2)) #f)
      (else
        (and (equal? (car l1) (car l2))
          (eqlist? (cdr l2) (cdr l2)))))))

;; Chapter 6. Shadows

(define numbered?
  (lambda (s)
    (cond
      ((atom? s) (number? s))
      (else
       (and (numbered? (car s))
         (numbered? (car (cdr (cdr s)))))))))

(define value-standart
  (lambda (nexp)
    (cond
      ((atom? nexp) nexp)
      ((eq? (car (cdr nexp)) '+)
       (plus (value-standart (car nexp))
             (value-standart (car (cdr (cdr nexp))))))
      ((eq? (car (cdr nexp)) '*)
       (mul (value-standart (car nexp)) (value-standart (car (cdr (cdr nexp))))))
      (else
       (pow (value-standart (car nexp)) (value-standart (car (cdr (cdr nexp)))))))))

;;Some help functions for the polish representation
;;of expresions.
(define operator
  (lambda (nexp)
    (car nexp)))

(define 1st-subexp
  (lambda (nexp)
    (car (cdr nexp))))

(define 2nd-subexp
  (lambda (nexp)
    (car (cdr (cdr nexp)))))
         
;;The function itself
(define value-polish
  (lambda (nexp)
    (cond
      ((atom? nexp) nexp)
      ((eq? (operator nexp) '+)
       (plus (value-polish (1st-subexp nexp))
             (value-polish (2nd-subexp nexp))))
      ((eq? (operator nexp) '*)
       (mul (value-polish (1st-subexp nexp))
             (value-polish (2nd-subexp nexp))))
      (else
       (pow (value-polish (1st-subexp nexp))
             (value-polish (2nd-subexp nexp)))))))


;; Chapter 7. Friends and Relations

(define set?
  (lambda (lat)
    (cond
      ((null? lat) #t)
      ((member? (car lat) (cdr lat)) #f)
      (else
       (set? (cdr lat))))))

(define makeset
  (lambda (lat)
    (cond
      ((null? lat) '())
      ((member? (car lat) (cdr lat))
        (makeset (cdr lat)))
      (else
            (cons (car lat)
              (makeset (cdr lat)))))))

(define subset?
  (lambda (set1 set2)
    (cond
      ((null? set1) #t)
      (else
       (and (member? (car set1) set2)
        (subset? (cdr set1) set2))))))

(define eqset?
  (lambda (set1 set2)
    (and (subset? set1 set2)
      (subset? set2 set1))))

(define intersect?
  (lambda (set1 set2)
    (cond
      ((null? set1) #f)
      ((member? (car set1) set2) #t)
      (else (intersect? (cdr set1) set2)))))

(define fancyIntersect?
  (lambda (set1 set2)
    (cond
      ((null? set1) #f)
      (else (or (member? (car set1) set2)
              (fancyIntersect?
                (cdr set1) set2))))))

(define intersect
  (lambda (set1 set2)
    (cond
      ((null? set1) '())
      ((member? (car set1) set2)
        (cons (car set1)
          (intersect (cdr set1)
            set2)))
      (else (intersect (cdr set1)
              set2)))))

(define union
  (lambda (set1 set2)
    (cond
      ((null? set1) set2)
      ((member? (car set1) set2)
       (union (cdr set1) set2))
      (else (cons (car set1)
              (union (cdr set1) set2))))))

(define intersectall
  (lambda (lset)
    (cond
      ((null? (cdr lset)) (car lset))
      (else
        (intersect (car lset)
         (intersectall (cdr lset)))))))

(define a-pair?
  (lambda (x)
    (cond
      ((atom? x) #f)
      ((null? x) #f)
      ((null? (cdr x)) #f)
      (else (null? (cdr (cdr x)))))))

(define (first x) (car x))

(define (second x) (car (cdr x)))

(define (third x) (car (cdr (cdr x))))

(define build
  (lambda (s1 s2)
    (cons s1 ( cons s2 '()))))

(define fun?
  (lambda (rel)
    (set? (firsts rel))))

(define revpair
  (lambda (pair)
    (build (second pair) (first pair))))

(define revrel
  (lambda (rel)
    (cond
      ((null? rel) '())
      (else
       (cons
        (revpair (car rel))
        (revrel (cdr rel)))))))

(define seconds
  (lambda (rel)
    (cond
      ((null? rel) '())
      (else
       (cons (second (car rel))
        (seconds (cdr rel)))))))

(define fullfun?
  (lambda (rel)
    (set? (seconds rel))))

(define rember-f-old
  (lambda (test? a lat)
    (cond
      ((null? lat) '())
      ((test? a (car lat))
       (rember-f test? a (cdr lat)))
      (else (cons (car lat)
              (rember-f test? a (cdr lat)))))))

(define eq?-c
  (lambda (a)
    (lambda (x)
      (eq? a x))))

;; Chapter 8. Lambda the Ultimate

(define rember-f
  (lambda (test?)
    (lambda (a lat)
      (cond
        ((null? lat) '())
        ((test? a (car lat))
         ((rember-f test?) a (cdr lat)))
        (else (cons (car lat)
                ((rember-f test?) a
                   (cdr lat))))))))

(define insertL-f
  (lambda (test?)
    (lambda (new old lat)
      (cond
        ((null? lat) '())
        ((test? old (car lat))
          (cons new
            (cons old
              ((insertL-f test?) new old (cdr lat)))))
        (else (cons (car lat)
                 ((insertL-f test?) new old
                   (cdr lat))))))))

(define insertR-f
  (lambda (test?)
    (lambda (new old lat)
      (cond
        ((null? lat) '())
        ((test? old (car lat))
         (cons old
           (cons new
             ((insertR-f test?) new old
              (cdr lat)))))
        (else (cons (car lat)
                ((insertR-f test?) new old
                 (cdr lat))))))))

(define seqL
  (lambda (new old l)
    (cons new
      (cons old l))))

(define seqR
  (lambda (new old l)
    (cons old (cons new l))))

(define insert-g
  (lambda (seq)
    (lambda (new old l)
      (cond
        ((null? l) '())
        ((eqan? old (car l))
         ((insert-g seq) new old (cdr l)))
        (else (cons (car l)
                ((insert-g seq) new old (cdr l))))))))

(define insertL-fancy (insert-g seqL))

(define insertR-fancy (insert-g seqR))

(define insertL-double-fancy
  (insert-g
    (lambda (new old l)
      (cons new (cons old l)))))

(define seqS
  (lambda (new old l)
    (cons new l)))

(define subst-fancy (insert-g seqS))

(define atom-to-function
  (lambda (x)
    (cond
      ((eq? x '+) plus)
      ((eq? x '*) mul)
      (else pow))))

(define value-fancy
  (lambda (nexp)
    (cond
      ((atom? nexp) nexp)
      (else
       ((atom-to-function
         (operator nexp))
             (value-fancy (1st-subexp nexp))
             (value-fancy (2nd-subexp nexp)))))))

(define multirember-f
  (lambda (test?)
    (lambda (a lat)
      (cond
        ((null? lat) '())
        ((test? a (car lat))
         ((multirember-f test?) a
          (cdr lat)))
        (else
          (cons (car lat)
            ((multirember-f test?) a
              (cdr lat))))))))

(define multirember-eq? (multirember-f eq?))

(define eq?-tuna
  (eq?-c 'tuna))

(define multiremberT
  (lambda (test?)
    (lambda (lat)
      (cond
        ((null? lat) '())
        ((test? (car lat))
         ((multiremberT test?) (cdr lat)))
        (else
          (cons (car lat)
            ((multiremberT test?) (cdr lat))))))))

(define multirember&co
  (lambda (a lat col)
    (cond
      ((null? lat)
       (col '() '()))
      ((eq? a (car lat))
       (multirember&co a
         (cdr lat)
         (lambda (newlat seen)
           (col newlat
             (cons (car lat) seen)))))
      (else
       (multirember&co a
         (cdr lat)
           (lambda (newlat seen)
             (col (cons (car lat) seen)
               seen)))))))

(define multiinsertLR
  (lambda (new oldL oldR lat)
    (cond
      ((null? lat) '())
      ((eq? oldL (car lat))
          (cons new
            (cons oldL
              (multiinsertLR new oldL oldR
                (cdr lat)))))
      ((eq? oldR (car lat))
          (cons oldR
            (cons new (multiinsertLR new
              oldL oldR (cdr lat)))))
      (else
       (cons (car lat) (multiinsertLR new oldL oldR (cdr lat)))))))

(define multiinsert&co
  (lambda (new oldL oldR lat col)
    (cond
      ((null? lat)
       (col '() 0 0))
      ((eq? oldL (car lat))
       (multiinsert&co new oldL oldR
         (cdr lat)
         (lambda (newlat L R)
           (col
             (cons new
               (cons oldL newlat))
             (add1 L) R))))
      ((eq? oldR (car lat))
       (multiinsert&co new oldL oldR
         (cdr lat)
         (lambda (newlat L R)
           (col
             (cons oldR
               (cons new newlat))
             L (add1 R)))))
      (else
        (multiinsert&co new oldL oldR
          (cdr lat)
          (lambda (newlat L R)
            (col (cons (car lat) newlat) L R)))))))

(define even?
  (lambda (n)
    (= (modulo n 2) 0)))

(define evens-only*
  (lambda (l)
    (cond
      ((null? l) '())
      ((atom? (car l))
       (cond
         ((even? (car l))
          (cons (car l) (evens-only* (cdr l))))
         (else
          (evens-only* (cdr l)))))
      (else
       (cons (evens-only* (car l))
             (evens-only* (cdr l)))))))


(define sum-prod-lst
  (lambda (ls sum product)
    (cons sum
      (cons product
        ls))))


;collects the list of lists of evens-only
;         the sum of all evens
;         the product of all odds
(define evens-only*&co
  (lambda (l col)
    (cond
      ((null? l)
       (col '() 0 1))
      ((atom? (car l))
       (cond
         ((even? (car l))
          (evens-only*&co (cdr l)
            (lambda (newls s p)
              (col (cons (car l) newls)
                   (plus (car l) s) p))))
         (else
          (evens-only*&co (cdr l)
            (lambda (newls s p)
              (col newls s
                (mul (car l) p)))))))
      (else
        (evens-only*&co (car l)
          (lambda (al as ap)
            (evens-only*&co (cdr l)
              (lambda (dl ds dp)
                (col (cons al dl)
                  (plus as ds)
                    (mul ap dp))))))))))

;; Chapter 9. ... and Again, and Again, and Again...

(define looking
  (lambda (a lat)
    (keep-looking a (car lat) lat)))

(define keep-looking
  (lambda (a sorn lat)
    (cond
      ((number? sorn)
        (keep-looking a
          (pick sorn lat) lat))
      ((eq? a sorn)))))

(define shift
  (lambda (pair)
    (build (first (first pair))
      (build (second (first pair))
         (second pair)))))

(define align
  (lambda (pora)
    (cond
      ((atom? pora) pora)
      ((a-pair? (first pora))
       (align (shift pora)))
      (else (build (first pora)
              (align (second pora)))))))

(define length*
  (lambda (pora)
    (cond
      ((atom? pora) 1)
      (else
       (plus (length* (first pora))
             (length* (second pora)))))))

(define weight*
  (lambda (pora)
    (cond
      ((atom? pora) 1)
      (else
       (plus (mul (weight* (first pora)) 2)
             (weight* (second pora)))))))

(define shuffle
  (lambda (pora)
    (cond
      ((atom? pora) pora)
      ((a-pair? (first pora))
       (shuffle (revpair pora)))
      (else (build (first pora)
              (shuffle (second pora)))))))
  
(define collatz
  (lambda (n)
    (cond
      ((one? n) 1)
      (else
       (cond
         ((even? n) (collatz (div n 2)))
         (else (collatz (add1 (mul n 3)))))))))

(define ackermann
  (lambda (m n)
    (cond
      ((zero? m) (add1 n))
      ((zero? n) (ackermann (sub1 m) 1))
      (else
        (ackermann (sub1 m)
          (ackermann m (sub1 n)))))))

(define eternity
  (lambda (x)
    (eternity x)))

(define tio
  (lambda (n)
    (cond
      ((eq? n 1) 1)
      (else (add1 n)))))

;; Applicative-order Y combinator
(define Y
  (lambda (le)
    ((lambda (f) (f f))
     (lambda (f)
       (le (lambda (x) ((f f) x)))))))

;; Chapter 10. What Is the Value of All of This?

(define new-entry build)

; Examples of entry creation
(define keys '(appetizer entree beverage))
(define values '(pate boeuf vin))
(define entry (new-entry keys values))

(define lookup-in-entry-help
    (lambda (name keys values entry-f)
      (cond
        ((null? keys) (entry-f name))
        ((eq? name (car keys))
          (car values))
        (else (lookup-in-entry-help name
          (cdr keys)
            (cdr values)
              entry-f)))))

(define lookup-in-entry
  (lambda (name entry entry-f)
    (lookup-in-entry-help name
      (first entry)
      (second entry)
      entry-f)))

(define extend-table cons)

#;(define lookup-in-table
  (lambda (name table table-f)
    (cond
      ((null? table) (table-f name))
      ((member? name (first (car table)))
       (lookup-in-entry name (car table) table-f))
      (else
        (lookup-in-table name
          (cdr table)
            table-f)))))

(define lookup-in-table
  (lambda (name table table-f)
    (cond
      ((null? table) (table-f name))
      (else (lookup-in-entry name
              (car table)
              (lambda (name)
                (lookup-in-table name
                  (cdr table) table-f)))))))

(define expression-to-action
  (lambda (e)
    (cond
      ((atom? e) (atom-to-action e))
      (else (list-to-action e)))))

(define atom-to-action
  (lambda (e)
    (cond
      ((number? e) *const)
      ((or (eq? e #t) (eq? e #f)) *const)
      ((eq? e 'car) *const)
      ((eq? e 'cons) *const)
      ((eq? e 'cdr) *const)
      ((eq? e 'null?) *const)
      ((eq? e 'eq?) *const)
      ((eq? e 'atom?) *const)
      ((eq? e 'cdr) *const)
      ((eq? e 'zero?) *const)
      ((eq? e 'add1) *const)
      ((eq? e 'sub1) *const)
      ((eq? e 'number?) *const)
      (else *identifier))))

(define list-to-action
  (lambda (e)
    (cond
      ((atom? (car e))
       (cond
         ((eq? (car e) 'quote)
          *quote)
         ((eq? (car e) 'lambda)
          *lambda)
         ((eq? (car e) 'cond)
          *cond)
         (else *application)))
      (else *application))))

(define value
  (lambda (e table)
    (meaning e table)))

(define meaning
  (lambda (e table)
    ((expression-to-action e) e table)))

(define *const
  (lambda (e table)
    (cond
      ((number? e) e)
      ((eq? e #t) #t)
      ((eq? e #f) #f)
      (else (build 'primitive e)))))

(define text-of second)

(define *quote
  (lambda (e table)
    (text-of e)))

(define *identifier
  (lambda (e table)
    ((lookup-in-table e table initial-table))))

(define initial-table
  (lambda (name)
    (car '(() a))))

(define *lambda
  (lambda (e table)
    (build 'non-primitive
      (cons table (cdr e)))))

(define table-of first)
(define formals-of second)
(define body-of third)

(define else?
  (lambda (e)
    (cond
      ((atom? e) (eq? e 'else))
      (else #f))))

(define question-of
  (lambda (line)
    (first line)))

(define answer-of
  (lambda (line)
    (second line)))

(define evcon
  (lambda (lines table)
    (cond
      ((else? (question-of (car lines)))
       (meaning (answer-of (car lines))
         table))
      ((meaning (question-of (car lines))
         table)
       (meaning (answer-of (car lines))
         table))
      (else (evcon (cdr lines) table)))))

(define *cond
  (lambda (e table)
    (evcon (cond-lines-of e) table)))

(define cond-lines-of cdr)

(define evlis
  (lambda (args table)
    (cond
      ((null? args) '())
      (else
        (cons (meaning (car args) table)
          (evlis (cdr args) table))))))

(define *application
  (lambda (e table)
    (apply
      (meaning (function-of e) table)
      (evlis (arguments-of e) table))))

(define function-of car)
(define arguments-of cdr)

(define primitive? first)
         
(define non-primitive? second)

(define apply
  (lambda (func values)
    (cond
      ((primitive? func)
       (apply-primitive (second func)
         values))
      ((non-primitive? func)
       (apply-non-primitive (second func)
         values)))))

(define apply-primitive
  (lambda (name vals)
    (cond
      ((eq? name 'cons)
       (cons (first vals) (second vals)))
      ((eq? name 'car)
       (car (first vals)))
      ((eq? name 'cdr)
       (cdr (first vals)))
      ((eq? name 'null?)
       (null? (first vals)))
      ((eq? name 'eq?)
       (eq? (first vals) (second vals)))
      ((eq? name 'atom?)
       (:atom? (first vals)))
      ((eq? name 'add1)
       (add1 (first vals)))
      ((eq? name 'sub1)
       (sub1 (first vals)))
      ((eq? name 'zero?)
       (zero? (first vals)))
      ((eq? name 'number?)
       (number? (first vals))))))

(define :atom?
  (lambda (e)
    (cond
      ((null? e) #f)
      ((atom? e) #t)
      ((or (eq? e 'primitive)
         (eq? e 'non-primitive)) #t)
      (else #f))))

(define apply-non-primitive
  (lambda (closure vals)
    (meaning (body-of closure)
      (extend-table
        (new-entry
          (formals-of closure)
          vals)
        (table-of closure)))))
       
      

        
            

