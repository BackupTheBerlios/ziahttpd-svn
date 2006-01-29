#!c:\bin\ruby -w

require 'net/http'

TestDirectory="./test/"
PsconDirectory="../pscon/"
RunFile="run"
DescFile = "desc"
$templateHTML=""

class UnitTest
  @templateStatus = ""
  @templateDesc = ""
  @templateReason  = ""
  @timeb = ""
  @timea = ""
  
  def initialize(path)
      @templateReason  = ""
      @templateDesc = ""
      if (File.exist?(path + RunFile))
        IO.foreach(path + RunFile){ |line|
          r = line.sub(/[ ]/, ' ' + path)
          @timeb = Time.now
          t = `#{r}`
          @timea = Time.now
          if ($?.exitstatus.to_i() == 255) 
            @templateReason = t.to_s()
            @templateStatus = "FAILED"
            if (File.exist?(path + DescFile))
              IO.foreach(path + DescFile){ |lineD|
                @templateDesc += lineD + "<br>"
              }
            end
          elsif ($?.exitstatus.to_i() == 0) 
            @templateStatus = "SUCCESS"
          elsif ($?.exitstatus.to_i() == 1) 
            @templateStatus = "NOT RUNNING"
          else
            @templateStatus = "UNKNOW"
          end
          templateGenerate(path)
          sendtoberlios(path)
        }
      else
        return false
      end
  end

  def sendtoberlios(path)
    to = ""
    h = Net::HTTP.new('localhost', 80)
    query = "tbegin=" + @timeb.to_i().to_s() + "&";
    query += "tend=" + @timea.to_i().to_s() + "&";
    query += "status=" + @templateStatus + "&";
    @templateReason.each_line {|line|
      to += line.chomp() + "<br>"
    }
    @templateReason = to
    query += "reason=" +@templateReason + "&";
    @templateDesc.each_line {|line|
      to += line.chomp() + "<br>"
    }
    @templateDesc = to
    query += "desc=" + @templateDesc + "&";
    query += "directory=" + path;
#    puts query
    data = h.get('/posttest.php?' + query);
    
  end
  
  def   fileexist(file)
    begin
      File.open( file , aModeString="r" )
    rescue
      return false
    else
      return true
    end
  end

  def   templateGenerate(path)
    if (@templateStatus == "SUCCESS")
      templateStatusColor = "#99FF99"
    end
    if (@templateStatus == "FAILED")
      templateStatusColor = "#FF9999"
    end
    if (@templateStatus == "UNKNOW")
      templateStatusColor = "yellow"
    end
    if (@templateStatus == "NOT RUNNING")
      templateStatusColor = "silver"
    end
    dur = @timea.to_i() - @timeb.to_i()
    puts "TEST: " + path + " " + @templateStatus
    @html = <<END_OF_STRING
    <tr>
      <td align=center>#{path}</td>
      <td align=center>#{@timeb.to_i()}</td>
      <td align=center>#{@timea.to_i()}</td>
      <td align=center>#{dur.to_i()}</td>
      <td align=center bgcolor="#{templateStatusColor}">#@templateStatus</td>
      <td align=center>#@templateReason</td>
      <td align=center>#@templateDesc</td>
    </tr>

END_OF_STRING
    $templateHTML += @html
  end

end

class Scroll
  def initialize(path)
    Dir.foreach(path) { |file|
      if (file.to_s()[0,1] != '.')
#        puts path + file + " is filea\n"
        if (!isdir(path + file))
          ut = UnitTest.new(path)
          return
        else
          Scroll.new(path + file + "/")
        end
      end
    }
    return
  end
  
  def isdir(file)
    if (File.ftype(file) == "directory")
      return true
    end
  return false
  end
end

$templateHTML = <<END_OF_STRING
    <html>
      <head>
        <title>LALALA <title>
      </head>
      <table border=1>
      <caption>Date : #{Time.now}</caption>
      <tr>
        <th>Test Directory</th>
        <th>Begin</th>
        <th>Stop</th>
        <th>Duration</th>
        <th>Status</th>
        <th>Reason</th>
        <th>Description</th>
      </tr>
END_OF_STRING
s = Scroll.new(TestDirectory)
$templateHTML += <<END_OF_STRING
    </table>
  </body>
</html>
END_OF_STRING
f = File.new("out.html", File::CREAT|File::TRUNC|File::RDWR, 0644)
f.print $templateHTML
f.close()
puts "Tester exit successfully"

