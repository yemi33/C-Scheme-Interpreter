; (letrec ((sum (lambda (x)
;                 (if (= x 0)
;                     0
;                     (+ x (sum (- x 1)))))))
;   (sum 5))

; (define x 3)
; (set! x 5)
; x

(letrec ((is-even? (lambda (n)
                       (or (= n 0)
                           (is-odd? (- n 1)))))
           (is-odd? (lambda (n)
                      (and (> n 0)
                           (is-even? (- n 1))))))
    (is-odd? 12))

;(or #t #f)

;;(and #t #t)

; (let ((x 3) (y 5))
;   (begin
;     x
;     y
;     (set! y 7)
;     (let ((z y))
;       y)))

