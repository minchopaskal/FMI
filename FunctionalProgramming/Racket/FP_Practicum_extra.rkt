#lang racket

;;Power function
(define pow
  (lambda (x n)
    (cond
      ((zero? n) 1)
      (else (* x (pow x (- n 1)))))))

;;Factoriel
(define fact
  (lambda (n)
    (cond
      ((= n 1) 1)
      (else (* n (fact (- n 1)))))))

;;Division (for we are smart (and fairly stupid)) 
(define div
  (lambda (a b)
    (cond
      ((< a b) 0)
      (else (+ 1 (div (- a b) b))))))

;;Задача 1. Да се дефинира процедура, която пресмята e^x по формулата на Тейлър.
(define (e-to x)
  (define (help n)
    (cond
      ((zero? n) 1)
      (else
       (+ (div (pow x n) (fact n))
            (help (- n 1))))))
  (help 1000))


;;Задача 2. Да се дефинира предикат (include? a b), който проверява дали числото a се съдържа в числото b (дали a е подниз на b). Напр., 123 е подниз на 5123783.
(define (include? a b)
  (define eq?-sub
    (lambda (a b)
      (cond
        ((< a 10) (= a (modulo b 10)) )
        (else
          (and
            (= (modulo b 10)
              (modulo a 10))
            (eq?-sub (div a 10)
              (quotient b 10)))))))
  (cond
    ((< b a) #f)
    (else
      (or
        (eq?-sub a b)
          (include? a (quotient b 10))))))

;;Задача 3. Да се дефинира процедура от по-висок ред (difference F a b), която по дадени едноаргументна реална функция F и две реални числа a и b намира разликата F(b) - F(a).
;; ОПА, дали не ни трябва за някоя от следващите задачи хмхмхмхм
(define (difference F a b)
  (- (F b) (F a)))

;;Задача 4. Чрез използване на lambda израз да се дефинира процедурен обект, който е еквивалентен на f, ако имаме дефиницията (define (f x) (* 2 x)).
(lambda (x)
  (* 2 x))

;;Задача 5. Да се дефинира процедура от по-висок ред (composition f g), която намира композицията на едноаргументните реални функции f и g.
(define composition
  (lambda (f g)
    (lambda (x)
      (f (g x)))))

;;Задача 6. Да се дефинира процедура от по-висок ред (derive f eps), която намира първа производна на едноаргументната реална функция f с точност eps.
(define derive
  (lambda (f eps)
    (lambda (x)
      (/
        (difference f x
          (+ x eps)) eps))))

;;Задача 7. Да се дефинира процедура от по-висок ред (derive2 f eps), която намира втора производна на едноаргументната реална функция f с точност eps.
(define derive2
  (lambda (f eps)
    (derive (derive f eps) eps)))

;;Задача 8. Да се дефинира процедура от по-висок ред (derive-n f n eps), която намира n-та производна на едноаргументната реална функция f с точност eps.
(define derive-n
  (lambda (f n eps)
    (cond
      ((= n 0) f)
      (else
        (derive
          (derive-n f (- n 1) eps) eps)))))

;;Задача 9. Да се дефинира процедура от по-висок ред (repeated f n), която намира n-кратна композиция на едноаргументната реална функция f.
(define repeated
  (lambda (f n)
    (cond
      ((= n 1) f)
      (else
        (composition f
          (repeated f (- n 1)))))))   

;;Simple function of one argument
(define timesTwo
  (lambda (x)
    (* x 2)))

;;Function demonstrating derivative above snd
(define pow-3
  (lambda (x)
    (pow x 3)))

