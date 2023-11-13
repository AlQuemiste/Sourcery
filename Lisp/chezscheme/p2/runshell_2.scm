;; Gemini

(import (rnrs)
        (rnrs mutable-strings)
        (rnrs io ports))

(define (run-command command)
  (let ((output-bytevector (open-output-bytevector))
        (error-bytevector (open-output-bytevector)))
    (with-output-to-ports (output-port output-bytevector)
                          (error-port error-bytevector)
      (system command))
    (get-output-string output-bytevector)  ; Store stdout as string
    (get-output-string error-bytevector))) ; Store stderr as string

; Example usage:
(let ((stdout-result (run-command "ls"))
      (stderr-result (run-command "some-command-with-error")))
  (display stdout-result)
  (display stderr-result))

