#!c:\bin\ruby -w

def write_erro()
  puts '<description>'

  puts '</description>'
end

require 'socket'
require 'net/http'

h = Net::HTTP.new('www.epitech.net', 80)
data = h.get('/', nil )
puts 'fsdfds'
exit(-1)
