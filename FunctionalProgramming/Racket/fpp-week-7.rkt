#lang racket
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Помощни макроси за "pretty-print" на примерите ;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define-syntax-rule (example stx)
  (if (string? stx)
      (printf "; ~a~n" stx)
      (printf "> ~s → ~v~n" 'stx stx)))

(define-syntax examples
  (syntax-rules ()
    [(examples x)
     (begin (example x) (displayln ""))]
    [(examples x y ...)
     (begin (example x) (examples y ...))]))
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; I. Функции от по-висок ред      ;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Примери:
(define (my-const x)
  (λ(_) x))
(define (my-compose f g)
  (λ(x) (f (g x))))
(define (derivative f eps)
  (λ(x) (/ (- (f (+ x eps)) (f (- x eps))) (* 2 eps))))



; Задача 1. Дефинирайте следните функции от по-висок ред:
; а). (fmin f g), която приема две едноместни числови функции f и gh
; и връща едноместни числова функция, чиято стойност в точка x е
; минимума на f и g.
; б). (fmax f g), като fmin, но връща максимума на f и g.
; в). (favg f g), като fmin, но връща средното аритметично на f и g.
(define (choose-f sf f g)
  (lambda (x)
    (sf (f x) (g x))))

(define (fmin f g) (choose-f min f g))

(define (fmax f g) (choose-f max f g))

(define avg
  (λ (x y)
    (/ (+ x y) 2)))

(define (favg f g) (choose-f avg f g))

(let ([f (λ(x) (* x x))]
      [g (λ(x) (+ x 1))])
  (examples
   ((fmin f g) 1)
   ((fmin f g) 10)
   ((fmax f g) 1)
   ((fmax f g) 10)
   ((favg f g) 1)
   ((favg f g) 10)
   ))


; Задача 2. Дефинирайте следните функции от по-висок ред:
; а). (bound-up f upper), която приема едноместни числова функция f и
; и числова стойност up и връща едноместни числова функция, чиято
; стойност в точка x е минимума на f(x) и up.
; б). (bound-down f lower), същата като bound-up, но връща максимума
; на f(x) и down.
(define (get-f sf f val)
  (lambda (x)
    (sf (f x) val)))

(define (bound-up f upper) (get-f min f upper))

(define (bound-down f lower) (get-f max f lower))

(let ([fup (bound-up (λ(x) (+ x 1)) 10)]
      [fdown (bound-down (λ(x) (* x x)) 5)])
  (examples
   (fup 0)
   (fup 10)
   (fdown 0)
   (fdown 10)
   ))
 

; Задача 3. Дефинирайте функцията (closest-point xys), която приема
; списък от точки в равнината (представени чрез двойки (x . y)) и връща
; едноаргументна функция, чиято стойност в дадена точка p e най-близката
; до p точка от xys.
(define (closest-point xys)
  (λ (point)
    (let ([xys-distances
           (map
            (lambda (p)
              (list p (sqrt (+ (sqr (- (car point) (car p)))
                               (sqr (- (cdr point) (cdr p)))))))
            xys)])
      (car (foldl
        (lambda (curr-point curr)
          (if (< (cadr curr-point) (cadr curr))
              curr-point
              curr))
        (car xys-distances)
        xys-distances)))))

(examples
 ((closest-point '((0 . 0) (0 . 10) (10 . 0) (10 . 10))) '(3 . 3))
 ((closest-point '((0 . 0) (0 . 10) (10 . 0) (10 . 10))) '(6 . 6))
 ((closest-point '((0 . 0) (0 . 10) (10 . 0) (10 . 10))) '(10 . 1))
 ((closest-point '((0 . 0) (0 . 10) (10 . 0) (10 . 10))) '(1 . 10))
 )


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; II. Асоциативни списъци и графи ;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


; Задача 4. Дефинирайте функцията (nodes edges), която приема
; списъс с ребрата edges на даден ориентиран граф (в който всяко
; ребро е представено като двойка (from . to)) и връща списък,
; съдържащ всички върхове на съответния граф.
(define (nodes edges)
  (reverse (foldl
    (lambda (edge acc)
      (cond
        ((member (car edge) acc)
          (cond
            ((member (cdr edge) acc) acc)
            (else (cons (cdr edge) acc))))
        ((member (cdr edge) acc) (cons (car edge) acc))
        (else (cons (cdr edge) (cons (car edge) acc)))))
    '()
    edges)))


; Задача 5. Дефинирайте функцията (adjacency-list edges), която приема
; списъс с ребрата edges на даден ориентиран граф (в който всяко
; ребро е представено като двойка (from . to)) и връща списъка на
; наследниците на съответния граф.
(define (get-neighbours node edges)
  (define (iter edges acc)
    (cond
      ((null? edges) acc)
      ((eq? node (caar edges))
        (iter (cdr edges)
          (cons
            (cdar edges) acc)))
      (else
        (iter (cdr edges)
          acc))))
  (iter edges '()))

;;; Reverse the neighbours for
;;; ascending ordering
(define (adjacency-list edges)
  (map
    (λ (node)
      (cons node
        (reverse
          (get-neighbours node
            edges))))
    (nodes edges)))


; Задача 6. Дефинирайте функцията (path? edges nodes), която приема
; списъс с ребрата edges на даден ориентиран граф и списък от върхове
; nodes и връща дали списъкът nodes е път в графа описан от edges.
(define (path? edges nodes)
  (cond
    ((null? (cdr nodes)) #t)
    (else (and (member (cons
      (car nodes) (cadr nodes)) edges)
        (path? edges
          (cdr nodes))))))


; Задача 7*. Дефинирайте функцията (simple-paths edges k from), която приема
; списъс с ребрата edges на даден ориентиран граф, цяло число k и идентификатор
; на връх node и връща всички прости пътища с дължина k, които започват от from.
(define (get-paths nodes edges)
  (map
    (λ (child)
      (cons child nodes))
    (foldl
      (λ (edge acc)
        (if (eq? (car edge) (car nodes))
            (cons (cdr edge) acc)
            acc))
      '()
      edges)))

(define (get-all-paths ln edges)
  (apply append
    (map
      (λ (path)
        (get-paths path edges))
      ln)))

(define (simple-paths edges k from)
  (define (iter edges acc k)
    (cond
      ((= k 1) (map reverse acc))
      (else (iter edges
        (get-all-paths acc edges)
          (- k 1)))))
  (iter edges (list (list from)) k))
               
; Задача 8*. Дефинирайте функцията (all-simple-paths edges from to), която приема
; списъс с ребрата edges на даден ориентиран граф и два идентификатора на върхове
; from и to и връща всички прости пътища, които започват от from и завършват в to.
(define TO-DO!! null)

(define (all-simple-paths edges from to) TO-DO!!)


; Примери върху графи
(let ([edges '((1 . 2) (1 . 3) (2 . 3) (3 . 2) (2 . 4) (3 . 5) (5 . 4))])
  (examples
   (nodes edges)
   (path? edges '(1 2 3 5))
   (path? edges '(1 2 5 3))
   (adjacency-list edges)
   (get-paths '(1) edges)
   (get-all-paths '((2 1) (3 1)) edges)
   (simple-paths edges 2 1)
   (simple-paths edges 3 1)
   (simple-paths edges 5 1)
   (all-simple-paths edges 1 4)
   (all-simple-paths edges 1 5)
   ))
  