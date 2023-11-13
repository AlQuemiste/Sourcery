;;; Chez-Scheme
;;; $ Write a simple well-commented efficient Scheme code with Chez Scheme dialect to run a shell command and capture stdout and stderr as two separate strings. Add an example to test this code. Use only functions and modules available in standard Chez Scheme library. The code must be usable on Windows, MacOS and Linux (platform independent).

;;; run-shell-command
;;;
;;; Executes a shell command and captures its stdout and stderr streams
;;; as two separate string values.
;;;
;;; @param command The shell command to execute as a string.
;;; @return Two string values: (values stdout-string stderr-string)
;;;
;;; This function is designed to be platform-independent and works on
;;; Windows, MacOS, and Linux.

(define run-shell-command
  (lambda (command)
    ;; The let-values form is used to bind the multiple return values of
    ;; open-process-ports to local variables.
    (let-values ([(to-stdin from-stdout-binary from-stderr-binary pid)
                  ;; open-process-ports starts the specified command in a new
                  ;; process. By default, it returns binary ports for stdout
                  ;; and stderr.
                  (open-process-ports command)])
      ;; To read strings, we must convert the binary ports to textual ports
      ;; using a transcoder. (native-transcoder) uses the system's default
      ;; character encoding, making the code portable.
      (let ([from-stdout (transcoded-port from-stdout-binary (native-transcoder))]
            [from-stderr (transcoded-port from-stderr-binary (native-transcoder))])
        (let ([stdout-string (get-string-all from-stdout)]
              [stderr-string (get-string-all from-stderr)])
              
          ;; The Role of End-of-File (EOF): The function `get-string-all` is a blocking operation. This means it will not return until it has read the entire contents of the stream. The function knows the stream has ended when it receives an end-of-file (EOF) signal.
          ;; How EOF is Triggered: For a pipe connected to a process's output, the EOF signal is sent under one main condition: the process on the writing end of the pipe closes it. A process automatically closes all of its open file descriptors (including stdout and stderr) when it terminates.
          ;; It's important to close all opened ports to release system resources.
          ;; Closing the transcoded port also closes the underlying binary port.
          (close-input-port from-stdout)
          (close-input-port from-stderr)
          (close-output-port to-stdin)
          ;; The function returns the captured stdout and stderr as two
          ;; separate string values.
          (values stdout-string stderr-string))))))

          
;;; Example to test the corrected run-shell-command function.
;;;
;;; This example executes a command that lists directory contents and
;;; prints the captured stdout and stderr.
(let-values ([(stdout stderr)
              ;; On Unix-like systems (Linux, MacOS), use "ls -a"
              ;; On Windows, you might use "dir /a"
              (run-shell-command "ls -a")])
  (display "--- Standard Output ---")
  (newline)
  (display stdout)
  (newline)
  (display "--- Standard Error ---")
  (newline)
  (display stderr)
  (newline))

;;; Example with a command that is expected to produce an error.
(let-values ([(stdout stderr)
              ;; "ls" on a non-existent file will write to stderr.
              (run-shell-command "ls non-existent-file.txt")])
  (display "--- Standard Output (Error Case) ---")
  (newline)
  (display stdout)
  (newline)
  (display "--- Standard Error (Error Case) ---")
  (newline)
  (display stderr)
  (newline))
