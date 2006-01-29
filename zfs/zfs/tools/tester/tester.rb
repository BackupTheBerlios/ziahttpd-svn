#!c:\bin\ruby -w

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
            @templateStatus = "FALSE"
            if (File.exist?(path + DescFile))
              IO.foreach(path + DescFile){ |lineD|
                @templateDesc += lineD + "<br>"
              }
            end
          elsif ($?.exitstatus.to_i() == 0) 
            @templateStatus = "OK"
          elsif ($?.exitstatus.to_i() == 1) 
            @templateStatus = "NOT RUNNING"
          else
            @templateStatus = "UNKNOW EXITSTATUS"
          end
          templateGenerate(path)
        }
      else
        return false
      end
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
    puts "TEST: " + path + " " + @templateStatus
    @html = <<END_OF_STRING
<div>
        TEST: #{path}<br>
        TEMPS DE DEBUT: #{@timeb.to_i()}<br>
        TEMPS DE FIN: #{@timea.to_i()}<br>
        STATUS: #@templateStatus<br>
        RAISON: #@templateReason<br>
        DESC: #@templateDesc<br>
</div>
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
END_OF_STRING
s = Scroll.new(TestDirectory)
$templateHTML += <<END_OF_STRING
  </body>
</html>
END_OF_STRING
f = File.new("out.html", File::CREAT|File::TRUNC|File::RDWR, 0644)
f.print $templateHTML
f.close()
puts "Tester exit successfully"

