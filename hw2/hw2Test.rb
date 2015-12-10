require 'net/http'
require 'optparse'
require 'tempfile'
require 'socket'

MIME = {
  :text_html  => "text/html",
  :text_css   => "text/css",
  :img_jpeg   => "image/jpeg",
  :app_js     => "application/javascript",
  :app_pdf    => "application/pdf",
  :app_octet  => "application/octet-stream",
}

FILES = {
  :html   => {:name => "web.html",       :mime => MIME[:text_html]},
  :css    => {:name => "style.css",      :mime => MIME[:text_css]},
  :image  => {:name => "image.jpeg",     :mime => MIME[:img_jpeg]},
  :js     => {:name => "script.js",      :mime => MIME[:app_js]},
  :pdf    => {:name => "doc.pdf",        :mime => MIME[:app_pdf]}
}

BADFILES = {
  :restricted =>  {:name => "restricted",             :mime => MIME[:app_octet]},
  :long       =>  {:name => "reallyLongFileName.txt", :mime => MIME[:app_octet]}
}

REQUIRED_HEADERS  = ["connection", "date", "server"]
GET_HEADERS       = REQUIRED_HEADERS + ["content-length", "content-type", "last-modified"]
POST_HEADERS      = REQUIRED_HEADERS + ["allow"]

def run_tests(host, server_root, local_root, port)
  total_possible = 130
  t1 = t2 = t3 = t4 = t5 = t6 = t7 = t8 = t9 = 0
  
  STDERR.puts "\n*** SERVER TEST ***\n"
  
  # ======== TEST 1 ========= #
  # GET 200 OK test
  STDERR.puts "\nTEST 1 - 200 OK (40 pts)"
  t1 += get_200_test(host, server_root, port, FILES[:html])
  STDERR.puts "Component Score -> #{t1}/40"
  # ========================= #  
  
  sleep 2

  # ======== TEST 2 ========= #
  # GET 200 OK test
  STDERR.puts "\nTEST 2 - 200 OK - MIME Types, Content, Header - (10 pts)"
  FILES.each do |k,v|
        t2 += get_200_test_deep_dive(host, server_root, local_root, port, v)
  end
  STDERR.puts "Component Score -> #{t2}/10"
  # ========================= #

  sleep 2

  # ======== TEST 3 ========= #
  # GET 400 Malformed test
  STDERR.puts "\nTEST 3 - 400 BAD REQUEST (10 pts)"
  t3 += get_400_test(host, port)
  STDERR.puts "Component Score -> #{t3}/10"
  # ========================= #

  sleep 2

  # ======== TEST 4 ========= #
  # GET 403 No Access
  STDERR.puts "\nTEST 4 - 403 NO ACCESS"
  t4 += get_403_test(host, server_root, port, BADFILES[:restricted])
  STDERR.puts "Component Score -> #{t4}/10"  
  # ========================= #

  sleep 2

  # ======== TEST 5 ========= #
  # GET 404 Not found
  STDERR.puts "\nTEST 5 - 404 NOT FOUND"
  t5 += get_404_test(host, port)
  STDERR.puts "Component Score -> #{t5}/10"  
  # ========================= #

  sleep 2

  # ======== TEST 6 ========= #
  # POST 405 Not allowed
  STDERR.puts "\nTEST 6 - 405 METHOD NOT SUPPORTED"
  t6 += post_405_test(host, server_root, port, FILES[:html])
  STDERR.puts "Component Score -> #{t6}/10"    
  # ========================= #

  sleep 2
  
  # ======== TEST 7 ========= #
  # GET 415 Uri length
  STDERR.puts "\nTEST 7 - 414 URI LENGTH"
  t7 += get_414_test(host, server_root, port, BADFILES[:long])
  STDERR.puts "Component Score -> #{t7}/10"      
  # ========================= #

  sleep 2

  # ======== TEST 8 ========= #
  # HEAD 200 Head
  STDERR.puts "\nTEST 8 - 200 OK HEAD"
  t8 += head_200_test(host, server_root, port, FILES[:html])
  STDERR.puts "Component Score -> #{t8}/10"      
  # ========================= #

  sleep 2
 
  STDERR.puts "\n*** CLIENT TEST ***\n"
  # ======== TEST 9 ========= #
  # Client Test
  STDERR.puts "\nTEST 9 - CLIENT (40 pts)"
  t9 += client_test
  STDERR.puts "Component Score -> #{t9}/40"      
  # ========================= #    
  
  final_score = t1+t2+t3+t4+t5+t6+t7+t8+t9
  STDERR.puts "\nFINAL SCORE = #{final_score}/150"
  return final_score
end

def client_test
  score = 0
  filename = "test.html"

  begin
    cmd = "./getfile http://www.clemson.edu/ces/computing/index.html -t 80 -f #{filename}"
    value = `#{cmd}`
  rescue => error
    STDERR.puts "\t__ERROR__ - Client crashed [FAIL]"
    STDERR.puts "\t#{error}"
    return score
  end
  
  file = File.open(filename, "r")
  contents = file.read
  
  if contents.include?("School of Computing")
    STDERR.puts "\tPASS - Client was successful in retrieving content [+40]"
    score += 40
  else 
    STDERR.puts "\t__ERROR__ - Client failed to get the web page! [FAIL]"
  end
  
  file.close

  rm = `rm -rf #{filename}`
  
  return score
end

def get_200_test(host, server_root, port, file)
  uri = URI("http://#{host}:#{port}/#{server_root}#{file[:name]}")
  STDERR.puts "GET: #{uri}\n"
  score = 0
  response = nil  

  # make the GET request
  begin
    Net::HTTP.start(uri.host, uri.port) do |http|
      STDERR.puts "\tPASS - Successfully connected to server on custom port [+5]"
      score += 5
      response = http.request(Net::HTTP::Get.new uri.path)
      http.finish    
    end
  rescue => error
    STDERR.puts "\t__ERROR__ - Could not successfully communicate with server [FAIL]\n"
    STDERR.puts "\t#{error}"
    return score
  end  
  
  # should succeed
  if response.code != "200"
    STDERR.puts "\t__ERROR__ - GET failed with code #{response.code} [FAIL]\n"
    return score
  end
  
  STDERR.puts "\tPASS - GET Successful [+35]"
  score += 35
  
  return score
end

# Tests for:
#   -success
#   -headers
#   -MIME type
#   -content
def get_200_test_deep_dive(host, server_root, local_root, port, file)
  uri = URI("http://#{host}:#{port}/#{server_root}#{file[:name]}")
  STDERR.puts "GET: #{uri}\n"
  score = 0
  response = nil  

  # make the GET request
  begin
    Net::HTTP.start(uri.host, uri.port) do |http|
      response = http.request(Net::HTTP::Get.new uri.path)
      http.finish  
    end
  rescue => error
    STDERR.puts "\t__ERROR__ - No Response or Incorrectly formatted response [FAIL]\n"
    STDERR.puts "\t#{error}"
    return score
  end
  
  # should succeed
  if response.code != "200"
    STDERR.puts "\t__ERROR__ - GET failed with code #{response.code} [FAIL]\n"
    return score
  end

  STDERR.puts "\tPASS - GET Successful [+2]"
  score += 2  

  # check for necessary headers
  missing_headers = check_headers(response, GET_HEADERS)  
  if missing_headers != []
    STDERR.puts "\t__ERROR__ - Missing required headers #{missing_headers} [-1]\n"
    score -= 1
  end
  
  content_type = response["content-type"]
  
  # check for correct MIME type
  if content_type != file[:mime]
    STDERR.puts "\t__ERROR__ - Incorrect MIME type - #{content_type} =/= #{file[:mime]} [-1]\n"
    score -= 1
  end
  
  # Make file out of response body
  tmpfile = Tempfile.new('response')
  tmpfile.write(response.body)
  tmpfile.flush

  # Open the original file
  file = File.new("#{local_root}/#{file[:name]}", "r")

  # Make sure the data is correct
  if not FileUtils.compare_file(file, tmpfile)
    STDERR.puts "\t__ERROR__ - Content Corrupted [-1]\n"
    score -= 1
  end
  
  # cleanup
  file.close
  tmpfile.close
  tmpfile.unlink
  
  return score > 0 ? score : 0
end

def get_400_test(host, port)
  request = "GET This will throw an error HTTP/1.1\r\n\r\n"
  score = 0
  response = nil
  
  begin
    socket = TCPSocket.open(host,port)  # Connect to server
    socket.print(request)               # Send request
    response = socket.read              # Read complete response
    socket.close
    STDERR.puts "\tPASS - Successfully communicated with server [+5]"
    score += 5
  rescue => error
    STDERR.puts "\t__ERROR__ - No Response or Incorrectly formatted response [FAIL]\n"
    STDERR.puts "\t#{error}"
    return score  
  end
  
  # Split response at first blank line into headers and body, then parse headers
  headers,body = response.split("\r\n\r\n", 2) 
  headers = response.split("\r\n")

  # did we get a 400?
  if not headers[0].include?("400")
    STDERR.puts "\t__ERROR__ - Should be a bad reuqest, instead - (#{headers[0]}) [FAIL]\n"
    return score
  end

  STDERR.puts "\tPASS - 400 test successful [+5]"
  score += 5

  return score
end

def get_403_test(host, server_root, port, file)
  uri = URI("http://#{host}:#{port}/#{server_root}#{file[:name]}")
  STDERR.puts "GET: #{uri}\n"
  score = 0
  response = nil
  
  # make the GET request
  begin
    Net::HTTP.start(uri.host, uri.port) do |http|
      response = http.request(Net::HTTP::Get.new uri.path)
      http.finish  
    end
    STDERR.puts "\tPASS - Successfully communicated with server [+5]"
    score += 5
  rescue => error
    STDERR.puts "\t__ERROR__ - No Response or Incorrectly formatted response [FAIL]\n"
    STDERR.puts "\t#{error}"
    return score
  end
  
  # should get a 403
  if response.code != "403"
    STDERR.puts "\t__ERROR__ - GET should fail with 403, instead #{response.code} [FAIL]\n"
    return score
  end

  # check for necessary headers
  missing_headers = check_headers(response, REQUIRED_HEADERS)  
  if missing_headers != []
    STDERR.puts "\t__ERROR__ - Missing required headers #{missing_headers}\n [-2]"
    score -= 2
  end

  STDERR.puts "\tPASS - 403 test successful [+5]"
  score += 5
  
  return score
end

def get_404_test(host, port)
  uri = URI("http://#{host}:#{port}/garbage.txt")
  STDERR.puts "GET: #{uri}\n"
  score = 0
  response = nil
 
  # make the GET request
  begin
    Net::HTTP.start(uri.host, uri.port) do |http|
      response = http.request(Net::HTTP::Get.new uri.path)
      http.finish  
    end
    STDERR.puts "\tPASS - Successfully communicated with server [+5]"
    score += 5
  rescue => error
    STDERR.puts "\t__ERROR__ - No Response or Incorrectly formatted response [FAIL]\n"
    STDERR.puts "\t#{error}"
    return score
  end
  
  # should get a 404
  if response.code != "404"
    STDERR.puts "\t__ERROR__ - GET should fail with 404, instead #{response.code} [FAIL]\n"
    return score
  end

  # check for necessary headers
  missing_headers = check_headers(response, REQUIRED_HEADERS)  
  if missing_headers != []
    STDERR.puts "\t__ERROR__ - Missing required headers #{missing_headers} [-2]\n"
    score -= 2
  end

  STDERR.puts "\tPASS - 404 test successful [+5]"
  score += 5
  
  return score
end

def post_405_test(host, server_root, port, file)
  uri = URI("http://#{host}:#{port}/#{server_root}#{file[:name]}")
  STDERR.puts "POST: #{uri}\n"
  score = 0
  response = nil
  
  # make the POST request
  begin
    Net::HTTP.start(uri.host, uri.port) do |http|
      response = http.request(Net::HTTP::Post.new uri.path)
      http.finish  
    end
    STDERR.puts "\tPASS - Successfully communicated with server [+5]"
    score += 5    
  rescue => error
    STDERR.puts "\t__ERROR__ - No Response or Incorrectly formatted response [FAIL]\n"
    STDERR.puts "\t#{error}"
    return score
  end
  
  # should get a 405
  if response.code != "405"
    STDERR.puts "\t__ERROR__ - POST should fail with 405, instead #{response.code} [FAIL]\n"
    return score
  end
  
  # check for necessary headers
  missing_headers = check_headers(response, POST_HEADERS)  
  if missing_headers != []
    STDERR.puts "\t__ERROR__ - Missing required headers #{missing_headers} [-2]\n"
    score -= 2
  end 

  STDERR.puts "\tPASS - 405 test successful [+5]"
  score += 5
  
  return score
end

def get_414_test(host, server_root, port, file)
  uri = URI("http://#{host}:#{port}/#{server_root}#{file[:name]}")
  STDERR.puts "GET: #{uri}\n"
  score = 0
  response = nil
  
  # make the GET request
  begin
    Net::HTTP.start(uri.host, uri.port) do |http|
      response = http.request(Net::HTTP::Get.new uri.path)
      http.finish  
    end
    STDERR.puts "\tPASS - Successfully communicated with server [+5]"
    score += 5
  rescue => error
    STDERR.puts "\t__ERROR__ - No Response or Incorrectly formatted response [FAIL]\n"
    STDERR.puts "\t#{error}"
    return score
  end
  
  # should get a 414
  if response.code != "414"
    STDERR.puts "\t__ERROR__ - GET should fail with 414, instead #{response.code} [FAIL]\n"
    return score
  end

  # check for necessary headers
  missing_headers = check_headers(response, REQUIRED_HEADERS)  
  if missing_headers != []
    STDERR.puts "\t__ERROR__ - Missing required headers #{missing_headers} [-5]\n"
    score -= 5
  end
  
  STDERR.puts "\tPASS - 414 test successful [+5]"
  score += 5
  
  return score
end

def head_200_test(host, server_root, port, file)
  uri = URI("http://#{host}:#{port}/#{server_root}#{file[:name]}")
  STDERR.puts "HEAD: #{uri}\n"
  score = 0
  response = nil
  
  # make the GET request
  begin
    Net::HTTP.start(uri.host, uri.port) do |http|
      response = http.request(Net::HTTP::Get.new uri.path)
      http.finish  
    end
    STDERR.puts "\tPASS - Successfully communicated with server [+5]"
    score += 5
  rescue => error
    STDERR.puts "\t__ERROR__ - No Response or Incorrectly formatted response [FAIL]\n"
    STDERR.puts "\t#{error}"
    return score
  end
  
  # should get a 200
  if response.code != "200"
    STDERR.puts "\t__ERROR__ - HEAD should succeed with 200, instead #{response.code} [FAIL]\n"
    return score
  end

  # check for necessary headers
  missing_headers = check_headers(response, GET_HEADERS)  
  if missing_headers != []
    STDERR.puts "\t__ERROR__ - Missing required headers #{missing_headers} [-5]\n"
    score -= 5
  end
  
  STDERR.puts "\tPASS - 200 HEAD test successful [+5]"
  score += 5
  
  return score
end

# returns list of headers that should be there but aren't
def check_headers(response, expected_headers)
  missing_headers = []
  
  expected_headers.each do |header|
    if response[header].nil?
      missing_headers << header
    end
  end
  
  return missing_headers
end

if __FILE__ == $0
  options = {:port => "80", :serv_dir => "", :local_dir => "./"}
  
  # Take in command line arguments
  OptionParser.new do |opts|
	  opts.banner = "Usage: server_test.rb -t [PORT] -s [SERV_DIR] -d [LOCAL_DIR]"
	  
	  opts.on('-t port', '--port port', String, 'Port') do |port|
		  options[:port] = port;
	  end

	  opts.on('-s serv', '--serv-dir serv', String, 'Server Root Directory') do |serv|
		  options[:serv_dir] = serv;
	  end
	  
	  opts.on('-d dir', '--dir dir', String, 'Local Root Directory') do |dir|
		  options[:dir] = dir;
	  end

	  opts.on('-h', '--help', 'Displays Help') do
		  STDERR.puts opts
		  exit
	  end
  end.parse!
  
  puts run_tests("localhost", options[:serv_dir], options[:dir], options[:port])
end
